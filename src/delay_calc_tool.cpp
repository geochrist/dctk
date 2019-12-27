#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdlib.h>
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
        {0,         0,                 0,  0 }
    };

    while  ((c = getopt_long(argc, argv, "l:s:c:x:", long_options, &option_index))) {

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
        case 'r':
            run_sims = true;
            break;
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
    } else {
        printf("Error:  Liberty option required!");
        exit(1);
    }
    if (read_lib_retval != 0) {
        printf("Error %d during Liberty processing.  Exiting.", read_lib_retval);
        exit(1);
    }

    cell_lib->dump();
    
    // Read Spef
    if (spef_file) {

        printf("Reading SPEF file %s\n", spef_file);
        spef = new spef::Spef();
        if (not spef->read(spef_file)) {
            std::cerr << "Error during SPEF processing" << *spef->error << std::endl;
            exit(1);
        }
    } else {
        printf("Error:  SPEF option required!");
        exit(1);
    }

    // Read Test Circuits
    if (test_circuits_file) {

        printf("Reading Test Circuits file %s\n", test_circuits_file);
        read_circuit_retval = read_circuits(test_circuits_file, &circuitMgr);
    } else {
        printf("Error:  Test circuits file required!");
        exit(1);
    }
    if (read_circuit_retval != 0) {
        printf("Error %d during Test Circuit processing.  Exiting.", read_circuit_retval);
        exit(1);
    }


    // Compute delays
    compute_delay_retval = compute_delays(cell_lib, &circuitMgr, spef);
    if (compute_delay_retval != 0) {
        printf("Error %d during delay calculation.  Exiting.", compute_delay_retval);
        exit(1);
    }

    // test code to see if values got captured
    for (std::size_t i = 0; i < circuitMgr.size(); i++) {
        // circuitMgr[i]->dump();
    }

    // create YAML buffer
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "Circuits";
    emitter << YAML::BeginSeq;
    for (std::size_t i = 0; i < circuitMgr.size(); i++) {
        circuitMgr[i]->gen_yaml(emitter);
    }
    emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

    // dump to debug file
    //std::ofstream yaml_fout("debug.circuits.yaml");
    //yaml_fout << emitter.c_str() << std::endl;
    //yaml_fout.close();

    if (spice_lib_name) {
        printf("Reading spice library %s\n", spice_lib_name);
        read_spice_lib(spice_lib_name, cell_lib);
    }

    // write spice decks to directory
    if (spice_dir_name) {

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


    // clean up
    if (liberty_file) {
        free(liberty_file);
    }

    if (spef_file) {
        free(spef_file);
    }

    if (cell_lib) {
        delete cell_lib;
    }

    exit(EXIT_SUCCESS);
}
