#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdlib.h>
#include <chrono>
#include <cmath>
#include <sys/time.h>   /* for rusage */
#include <sys/resource.h> /* for rusage */

#include <yaml-cpp/yaml.h>
#include <parser-spef.hpp>

// dctk definitions
#include "dctk.hpp"

// Liberty reader
#include "liberty_reader.hpp"

// Test circuit reader
#include "circuit_reader.hpp"

// Delay calculator
#include "delay_calculator.hpp"

// SPICE lib reader
#include "spice_lib_reader.hpp"

// Benchmarks object definition
#include "Benchmarks.hpp"

//
// Usage:
//
// delay_calc_tool [options]
//
//   --liberty LIBERTY    Liberty model
//   --circuits CIRCUITS  Test Circuits file

//
// Format for circuits file (using YAML format)
//
// Circuits:
//   -
//     name: circuitname1
//     voltage_source:
//     driver: <celltype/input/output>
//     load:  <celltype/input>
//     load_interconnect: <c1> <r> <c2>
//   -
//     name: circuitname2
//     ...
//

// constants needed for scoring
const float DELAY_PTS = 40.0 ;
const float SLEW_PTS = 60.0 ;
const float DELAY_PEN = 20.0 ;
const float SLEW_PEN = 30.0 ;
const float DELAY_TA = 2.0 ; // units of ps
const float SLEW_TA = 4.0 ; // units of ps
const float DELAY_TO = 0.05 ;
const float SLEW_TO = 0.10 ;


//
// Container for rise/fall delay/slew and all of the repeated calculation logic
//
struct delay_slew
{
    // data
    static const unsigned RISE = 0, FALL = 1;
    float delay[2];
    float slew[2];
    delay_slew(float d_R, float d_F, float s_R, float s_F)
        : delay{d_R, d_F}, slew{s_R, s_F} {}
    // Checks if all 4 enties are valid
    bool is_valid(void) {
        return (delay[RISE] >= -1.0 && delay[FALL] >= -1.0 &&
                slew[RISE] >= -1.0 && slew[FALL] >= -1.0);
    }
};

//
// Calculates relative diffs of ccs delay/slew measurements from spice,
// and updates RMS accumulators and outlier counters
//
void calculate_dx(const delay_slew& spice, const delay_slew& ccs,
                  float* acc_delay_dx, float *acc_slew_dx,
                  unsigned* NO_delay, unsigned* NO_slew)
{
    for (unsigned rf : {delay_slew::RISE, delay_slew::FALL}) {
        // delay
        float delay_dx = std::abs(ccs.delay[rf] - spice.delay[rf]);
        if (delay_dx > DELAY_TA) {
            delay_dx = delay_dx / std::abs(spice.delay[rf]);
            (*acc_delay_dx) += pow(delay_dx, 2);
            (*NO_delay) += (delay_dx > DELAY_TO);
        }
        // slew
        float slew_dx = std::abs(ccs.slew[rf] - spice.slew[rf]);
        if (slew_dx > SLEW_TA) {
            slew_dx = slew_dx / std::abs(spice.slew[rf]);
            (*acc_slew_dx) += pow(slew_dx, 2);
            (*NO_slew) += (slew_dx > SLEW_TO);                    
        }
    }
    return;
}

//
// Implements the scoring algorithm described in the TAU2021 contest rules
//
void analyze_results(dctk::CircuitPtrVec& circuitMgr, dctk::Benchmarks* benchmarks)
{
    const unsigned CELL = 0, NET = 1;

    // accumulated diffs for RMS calc (CELL, NET)
    float accumulated_delay_dx[2] = { 0.0, 0.0 };
    float accumulated_slew_dx[2] = { 0.0, 0.0 };

    // outlier counts (CELL, NET)
    unsigned NO_delay[2] = { 0, 0 };
    unsigned NO_slew[2] = { 0, 0 };
   
    unsigned skipped_circuits = 0;

    // go ..
    for (std::size_t i = 0; i < circuitMgr.size(); i++) {

        // spice data
        delay_slew spice_cell(circuitMgr[i]->get_spice_cell_rise_delay(),
                              circuitMgr[i]->get_spice_cell_fall_delay(),
                              circuitMgr[i]->get_spice_cell_rise_slew(),
                              circuitMgr[i]->get_spice_cell_fall_slew());
        
        delay_slew spice_net(circuitMgr[i]->get_spice_net_rise_delay(),
                             circuitMgr[i]->get_spice_net_fall_delay(),
                             circuitMgr[i]->get_spice_net_rise_slew(),
                             circuitMgr[i]->get_spice_net_fall_slew());

        // if any of the spice results are invalid, we skip the circuit from inclusion
        if (!spice_cell.is_valid() || !spice_net.is_valid()) {
            skipped_circuits++;
            std::cout << "Ignore circuit " << circuitMgr[i]->get_name() << std::endl;
            continue;
        }

        // ccs data
        delay_slew ccs_cell(circuitMgr[i]->get_ccs_cell_rise_delay(),
                            circuitMgr[i]->get_ccs_cell_fall_delay(),
                            circuitMgr[i]->get_ccs_cell_rise_slew(),
                            circuitMgr[i]->get_ccs_cell_fall_slew());
        delay_slew ccs_net(circuitMgr[i]->get_ccs_net_rise_delay(),
                           circuitMgr[i]->get_ccs_net_fall_delay(),
                           circuitMgr[i]->get_ccs_net_rise_slew(),
                           circuitMgr[i]->get_ccs_net_fall_slew());


        // calculate diffs and update outlier counts
        calculate_dx(spice_cell, ccs_cell,
                     &accumulated_delay_dx[CELL], &accumulated_slew_dx[CELL],
                     &NO_delay[CELL], &NO_slew[CELL]);
        calculate_dx(spice_net, ccs_net,
                     &accumulated_delay_dx[NET], &accumulated_slew_dx[NET],
                     &NO_delay[NET], &NO_slew[NET]);

    }

    // tally up
    float N = 2.0*(circuitMgr.size() - skipped_circuits);
    float measAccuracy_delay[2];
    float measAccuracy_slew[2];
    float measPTS_delay[2];
    float measPTS_slew[2];
    for (unsigned cell_net : { CELL, NET }) {
        measAccuracy_delay[cell_net] = sqrt(accumulated_delay_dx[cell_net] / N);
        measAccuracy_slew[cell_net] = sqrt(accumulated_slew_dx[cell_net] / N);
        measPTS_delay[cell_net] =
            (1.0 - 4.0 * measAccuracy_delay[cell_net]) * DELAY_PTS - NO_delay[cell_net]/N * DELAY_PEN;        
        measPTS_slew[cell_net] =
            (1.0 - 4.0 * measAccuracy_slew[cell_net]) * SLEW_PTS - NO_slew[cell_net]/N * SLEW_PEN;
    }
    
    // store results
    benchmarks->cell_rms_delay_diff = measAccuracy_delay[CELL];
    benchmarks->cell_rms_slew_diff = measAccuracy_slew[CELL];
    benchmarks->cell_delay_outliers = NO_delay[CELL];
    benchmarks->cell_slew_outliers = NO_slew[CELL];
    benchmarks->cell_delay_pts = measPTS_delay[CELL];
    benchmarks->cell_slew_pts = measPTS_slew[CELL];
    benchmarks->net_rms_delay_diff = measAccuracy_delay[NET];
    benchmarks->net_rms_slew_diff = measAccuracy_slew[NET];
    benchmarks->net_delay_outliers = NO_delay[NET];
    benchmarks->net_slew_outliers = NO_slew[NET];
    benchmarks->net_delay_pts = measPTS_delay[NET];
    benchmarks->net_slew_pts = measPTS_slew[NET];
}

//
// Implements the scoring algorithm described in the TAU2020 contest rules -
// does not take into account net delay/slew. Here for backward compat.
//
void analyze_results_2020(dctk::CircuitPtrVec& circuitMgr, dctk::Benchmarks* benchmarks) {

    // average_delay_diff = average(abs(ccs_*_delay - spice_*_delay)/spice_*_delay)*100 [change to %]
    // slew_benchmark = average(abs(ccs_*_slew - spice_*_slew)/spice_*_slew)*100 [change to %]

    float accumulated_delay_diff = 0.0;
    float accumulated_slew_diff = 0.0;
    int NO_delay = 0;
    int NO_slew = 0;

    int skipped_circuits = 0;


    for (std::size_t i = 0; i < circuitMgr.size(); i++) {

        // if any of the spice results are negative, we skip the circuit from inclusion

        const float spice_cell_rise_delay = circuitMgr[i]->get_spice_cell_rise_delay();
        const float spice_cell_fall_delay = circuitMgr[i]->get_spice_cell_fall_delay();
        const float spice_cell_rise_slew = circuitMgr[i]->get_spice_cell_rise_slew();
        const float spice_cell_fall_slew = circuitMgr[i]->get_spice_cell_fall_slew();


        const float spice_net_rise_delay = circuitMgr[i]->get_spice_net_rise_delay();
        const float spice_net_fall_delay = circuitMgr[i]->get_spice_net_fall_delay();
        const float spice_net_rise_slew = circuitMgr[i]->get_spice_net_rise_slew();
        const float spice_net_fall_slew = circuitMgr[i]->get_spice_net_fall_slew();

	if ((spice_cell_rise_delay < -1.0) || (spice_cell_fall_delay < -1.0) ||
            (spice_cell_rise_slew < -1.0) || (spice_cell_fall_slew < -1.0)) {
            skipped_circuits++;
            std::cout << "Ignore circuit " << circuitMgr[i]->get_name() << std::endl;
            continue;
        }

	if ((spice_net_rise_delay < -1.0) || (spice_net_fall_delay < -1.0) ||
            (spice_net_rise_slew < -1.0) || (spice_net_fall_slew < -1.0)) {
            skipped_circuits++;
            std::cout << "Ignore circuit " << circuitMgr[i]->get_name() << std::endl;
            continue;
        }

        // delay
        const float ccs_cell_rise_delay = circuitMgr[i]->get_ccs_cell_rise_delay();
        const float ccs_cell_fall_delay = circuitMgr[i]->get_ccs_cell_fall_delay();

        // check for thresholding
        float dx_cell_delay_rise;
        float dx_cell_delay_fall;
        if (std::abs(ccs_cell_rise_delay - spice_cell_rise_delay) > DELAY_TA) {
            dx_cell_delay_rise = std::abs(ccs_cell_rise_delay - spice_cell_rise_delay)/std::abs(spice_cell_rise_delay) ;
        } else {
            dx_cell_delay_rise = 0.0;
        }
        if (std::abs(ccs_cell_fall_delay - spice_cell_fall_delay) > DELAY_TA) {
            dx_cell_delay_fall = std::abs(ccs_cell_fall_delay - spice_cell_fall_delay)/std::abs(spice_cell_fall_delay) ;
        } else {
            dx_cell_delay_fall = 0.0;
        }

        accumulated_delay_diff += pow(dx_cell_delay_rise, 2);
        accumulated_delay_diff += pow(dx_cell_delay_fall, 2);

        // count outliers
        if (dx_cell_delay_rise > DELAY_TO) {
            NO_delay++;
        }
        
        if (dx_cell_delay_fall > DELAY_TO) {
            NO_delay++;
        }

        // delay
        const float ccs_cell_rise_slew = circuitMgr[i]->get_ccs_cell_rise_slew();
        const float ccs_cell_fall_slew = circuitMgr[i]->get_ccs_cell_fall_slew();

        // check for thresholding
        float dx_cell_slew_rise;
        float dx_cell_slew_fall;
        if (std::abs(ccs_cell_rise_slew - spice_cell_rise_slew) > SLEW_TA) {
            dx_cell_slew_rise = std::abs(ccs_cell_rise_slew - spice_cell_rise_slew)/std::abs(spice_cell_rise_slew) ;
        } else {
            dx_cell_slew_rise = 0.0;
        }
        if (std::abs(ccs_cell_fall_slew - spice_cell_fall_slew) > SLEW_TA) {
            dx_cell_slew_fall = std::abs(ccs_cell_fall_slew - spice_cell_fall_slew)/std::abs(spice_cell_fall_slew) ;
        } else {
            dx_cell_slew_fall = 0.0;
        }

        accumulated_slew_diff += pow(dx_cell_slew_rise, 2);
        accumulated_slew_diff += pow(dx_cell_slew_fall, 2);
        // count outliers
        if (dx_cell_slew_rise > SLEW_TO) {
            NO_slew++;
        }
        
        if (dx_cell_slew_fall > SLEW_TO) {
            NO_slew++;
        }

    }

    const float N = 2.0*(circuitMgr.size() - skipped_circuits);
    const float measAccuracy_delay = sqrt(accumulated_delay_diff / (N) );
    const float measAccuracy_slew = sqrt(accumulated_slew_diff / (N) );
    const float measPTS_delay = (1.0 - 2.0 * measAccuracy_delay) * DELAY_PTS - NO_delay/N * DELAY_PEN;
    const float measPTS_slew = (1.0 - 2.0 * measAccuracy_slew) * SLEW_PTS - NO_slew/N * SLEW_PEN;
    
    // store results
    benchmarks->cell_rms_delay_diff = measAccuracy_delay;
    benchmarks->cell_rms_slew_diff = measAccuracy_slew;
    benchmarks->cell_delay_outliers = NO_delay;
    benchmarks->cell_slew_outliers = NO_slew;
    benchmarks->cell_delay_pts = measPTS_delay;
    benchmarks->cell_slew_pts = measPTS_slew;
}


int
main(int argc, char **argv)
{
    int c;

    // Liberty
    char* liberty_file = nullptr;
    dctk::CellLib* cell_lib = nullptr;
    int read_lib_retval = 0;

    // SPEF
    char* spef_file = nullptr;
    spef::Spef* spef = nullptr;

    // Test Circuits
    char* test_circuits_file = nullptr;
    dctk::CircuitPtrVec circuitMgr;
    int read_circuit_retval = 0;

    // Compute Delays
    int compute_delay_retval = 0;

    // spice decks
    char* spice_dir_name = nullptr;

    // spice library
    char* spice_lib_name = nullptr;

    // spice models
    char* spice_models = nullptr;
    
    // simulator
    char* simulator = nullptr;
    
    // generate spice decks
    bool gen_spice_decks = false;
    
    // run simulation
    bool run_sims = false;
    
    // merged circuit file (output)
    char* merged_circuits = nullptr;

    // run delay calculation
    char* dc_file = nullptr;
    
    // benchmarks
    dctk::Benchmarks benchmarks;
    
    // rusage
    rusage rusage_before ;
    rusage rusage_after ;

    // get options
    int option_index = 0;
    static struct option long_options[] = {
        {"liberty", required_argument, 0, 'l'},
        {"spef", required_argument, 0, 's'},
        {"circuits", required_argument, 0, 'c'},
        {"spice_dir", required_argument, 0, 'd'},
        {"spice_lib", required_argument, 0, 'x'},
        {"spice_models", required_argument, 0, 'm'},
        {"simulator", required_argument, 0, 'z'},
        {"gen_spice_decks", no_argument, 0, 'g'},
        {"run_sims", no_argument, 0, 'r'},
        {"dc_file", required_argument, 0, 'y'},
        {"merged_circuits", required_argument, 0, 'e'},
        {0,         0,                 0,  0 }
    };

    while  ((c = getopt_long(argc, argv, "l:s:c:d:x:m:z:rye:", long_options, &option_index))) {

        if (c == -1)
            break;

        switch (c) {
        case 'l':
            liberty_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(liberty_file, optarg);
            break;
        case 'c':
            test_circuits_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(test_circuits_file, optarg);
            break;
        case 's':
            spef_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spef_file, optarg);
            break;
        case 'd':
            spice_dir_name = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spice_dir_name, optarg);
            break;
        case 'x':
            spice_lib_name = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spice_lib_name, optarg);
            break;
        case 'm':
            spice_models = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spice_models, optarg);
            break;
        case 'z':
            simulator = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(simulator, optarg);
            break;
        case 'e':
            merged_circuits = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(merged_circuits, optarg);
            break;
        case 'r':
            run_sims = true;
            break;
        case 'g':
            gen_spice_decks = true;
            break;
        case 'y':
            dc_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(dc_file, optarg);
        case '?':
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    // Read Liberty
    if (liberty_file) {

        printf("Reading Liberty file %s\n", liberty_file);
        read_lib_retval = read_liberty(liberty_file, cell_lib);
        if (read_lib_retval != 0) {
            printf("Error %d during Liberty processing.  Exiting.", read_lib_retval);
            exit(1);
        }
        cell_lib->dump();
    }

    
    // Read Spef
    if (spef_file) {

        printf("Reading SPEF file %s\n", spef_file);
        spef = new spef::Spef();
        if (not spef->read(spef_file)) {
            std::cerr << "Error during SPEF processing" << *spef->error << std::endl;
            exit(1);
        }
    }

    // Read Test Circuits
    if (test_circuits_file) {

        printf("Reading Test Circuits file %s\n", test_circuits_file);
        read_circuit_retval = read_circuits(test_circuits_file, &circuitMgr, &benchmarks);

        if (read_circuit_retval != 0) {
            printf("Error %d during Test Circuit processing.  Exiting.", read_circuit_retval);
            exit(1);
        }
    }


    // Compute delays
    if (dc_file) {

        // benchmarking before computation
        getrusage(RUSAGE_SELF, &rusage_before);
        auto elapsed_t1 = std::chrono::steady_clock::now();

        // computation
        compute_delay_retval = compute_delays(cell_lib, &circuitMgr, spef);
        if (compute_delay_retval != 0) {
            printf("Error %d during delay calculation.  Exiting.", compute_delay_retval);
            exit(1);
        }

        // benchmarking after computation
        getrusage(RUSAGE_SELF, &rusage_after);
        auto elapsed_t2 = std::chrono::steady_clock::now();

        // elapsed time
        std::chrono::duration<float> elapsed_time = elapsed_t2 - elapsed_t1;

        // total user time
        float user_time_after = (float) rusage_after.ru_utime.tv_sec + 0.000001*(float)rusage_after.ru_utime.tv_usec;
        float user_time_before = (float) rusage_before.ru_utime.tv_sec + 0.000001*(float)rusage_before.ru_utime.tv_usec;
        float net_user_time = user_time_after - user_time_before;

        // total system time
        float system_time_after = (float) rusage_after.ru_stime.tv_sec + 0.000001*(float)rusage_after.ru_stime.tv_usec;
        float system_time_before = (float) rusage_before.ru_stime.tv_sec + 0.000001*(float)rusage_before.ru_stime.tv_usec;
        float net_system_time = system_time_after - system_time_before;
        
        // store results
        benchmarks.elapsed_time = elapsed_time.count();
        benchmarks.cpu_time = net_user_time + net_system_time;
        std::cout << "memory after = " << rusage_after.ru_maxrss << "; memory before = " << rusage_before.ru_maxrss << std::endl;

        benchmarks.incremental_memory = (float) rusage_after.ru_maxrss - (float) rusage_before.ru_maxrss; 

        // write out results file

        // format data
        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Circuits";
        emitter << YAML::BeginSeq;
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {
            circuitMgr[i]->gen_yaml(emitter);
        }
        emitter << YAML::EndSeq;
        benchmarks.dump_yaml(emitter);
        emitter << YAML::EndMap;

        // write to file

        std::ofstream yaml_fout(dc_file);
        yaml_fout << emitter.c_str() << std::endl;
        yaml_fout.close();

        
    }

    // prepare for spice simulations
    if (spice_lib_name) {
        printf("Reading spice library %s\n", spice_lib_name);
        read_spice_lib(spice_lib_name, cell_lib);
    }

    // write spice decks to directory
    if (spice_dir_name && gen_spice_decks) {

        printf("Writing spice decks into %s\n", spice_dir_name);
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {

            circuitMgr[i]->write_spice_deck(std::string(spice_dir_name),
                                            cell_lib, spef, spice_lib_name, spice_models,
                                            simulator);
        }
    }

    // run simulations if requested
    if (run_sims) {
        printf("Running simulations\n");
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {
            std::string deck_name = std::string(spice_dir_name) + "/" + circuitMgr[i]->get_name() + ".sp";
            printf("Running simulation for %s\n", deck_name.c_str() );

            if (!strcmp(simulator,"xyce")) {
                std::string cmd = "Xyce -l " + deck_name + ".log " + deck_name;
                system(cmd.c_str());
            }
            
            if (!strcmp(simulator,"ngspice")) {
                std::string cmd = "ngspice -b -o " + deck_name + ".log " + deck_name;
                system(cmd.c_str());
            }

        }

    }
    
    // merge simulation results into Circuits
    if (spice_dir_name && merged_circuits) {
        printf("Merging golden results from %s\n", spice_dir_name);
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {

            // merge results for only recognized simulators
            if (!strcmp(simulator,"xyce")) {
                std::string results_file_name = std::string(spice_dir_name) + "/" + circuitMgr[i]->get_name() + ".sp.mt0";
                printf("Reading simulation results %s\n", results_file_name.c_str() );
                circuitMgr[i]->read_spice_results(simulator, results_file_name);
            }

            if (!strcmp(simulator,"ngspice")) {
                std::string results_file_name = std::string(spice_dir_name) + "/" + circuitMgr[i]->get_name() + ".sp.log";
                printf("Reading simulation results %s\n", results_file_name.c_str() );
                circuitMgr[i]->read_spice_results(simulator, results_file_name);
            }
            
        }

        // analyze accuracy of results
        analyze_results(circuitMgr, &benchmarks);

        // format data
        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Circuits";
        emitter << YAML::BeginSeq;
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {
            circuitMgr[i]->gen_yaml(emitter);
        }
        emitter << YAML::EndSeq;
        benchmarks.dump_yaml(emitter);
        emitter << YAML::EndMap;

        // write to file

        std::ofstream yaml_fout(merged_circuits);
        yaml_fout << emitter.c_str() << std::endl;
        yaml_fout.close();

    }

    // clean up
    if (test_circuits_file) {
        free(test_circuits_file);
        test_circuits_file = nullptr;
    }

    if (liberty_file) {
        free(liberty_file);
    }

    if (spef_file) {
        free(spef_file);
        spef_file = nullptr;
    }

    if (cell_lib) {
        delete cell_lib;
        cell_lib = nullptr;
    }

    if (spice_dir_name) {
        free(spice_dir_name);
        spice_dir_name = nullptr;
    }

    if (spice_lib_name) {
        free(spice_lib_name);
        spice_lib_name = nullptr;
    }

    if (spice_models) {
        free(spice_models);
        spice_models = nullptr;
    }

    if (simulator) {
        free(simulator);
        simulator = nullptr;
    }

    if (merged_circuits) {
        free(merged_circuits);
        merged_circuits = nullptr;
    }

    if (dc_file) {
        free(dc_file);
        dc_file = nullptr;
    }

    exit(EXIT_SUCCESS);
}
