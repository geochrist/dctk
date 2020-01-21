#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdlib.h>
#include <chrono>
//#include <algorithm>

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
    
    // run simulation
    bool run_sims = false;
    
    // merged circuit file (output)
    char* merged_circuits = nullptr;

    // run delay calculation
    char* dc_file = nullptr;
    
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
        read_circuit_retval = read_circuits(test_circuits_file, &circuitMgr);

        if (read_circuit_retval != 0) {
            printf("Error %d during Test Circuit processing.  Exiting.", read_circuit_retval);
            exit(1);
        }
    }


    // Compute delays
    if (dc_file) {
        auto elapsed_t1 = std::chrono::steady_clock::now();
        clock_t cpu_time_start = clock();
        compute_delay_retval = compute_delays(cell_lib, &circuitMgr, spef);
        if (compute_delay_retval != 0) {
            printf("Error %d during delay calculation.  Exiting.", compute_delay_retval);
            exit(1);
        }
        auto elapsed_t2 = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed_time = elapsed_t2 - elapsed_t1;
        clock_t cpu_time_ticks = clock() - cpu_time_start ;
        double cpu_time = (float) cpu_time_ticks / (float) CLOCKS_PER_SEC ; 

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
        emitter << YAML::Key << "Statistics";
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "elasped_time" << YAML::Value << elapsed_time.count() ;
        emitter << YAML::Key << "cpu_time" << YAML::Value << (float) cpu_time ;
        emitter << YAML::EndMap;
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
    if (spice_dir_name && run_sims) {

        printf("Writing spice decks into %s\n", spice_dir_name);
        for (std::size_t i = 0; i < circuitMgr.size(); i++) {

            circuitMgr[i]->write_spice_deck(std::string(spice_dir_name),
                                            cell_lib, spef, spice_lib_name, spice_models,
                                            simulator);
        }
    }

    // run simulations if reqested
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
