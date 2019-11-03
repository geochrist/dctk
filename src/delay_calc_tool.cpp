#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <optional>
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
    bool read_liberty_file = false;
    char* liberty_file = NULL;
    dctk::CellLib* cell_lib = NULL;
    int read_lib_retval = 0;

    // SPEF
    bool read_spef_file = false;
    char* spef_file = NULL;
    spef::Spef* spef = NULL;

    // Test Circuits
    bool read_test_circuits_file = false;
    char* test_circuits_file = NULL;
    dctk::CircuitPtrVec circuitMgr;
    int read_circuit_retval = 0;

    // Compute Delays
    int compute_delay_retval = 0;

    // get options
    int option_index = 0;
    static struct option long_options[] = {
        {"liberty", required_argument, 0, 'l'},
        {"spef", required_argument, 0, 's'},
        {"circuits", required_argument, 0, 'c'},
        {0,         0,                 0,  0 }
    };

    while  ((c = getopt_long(argc, argv, "l:s:c:", long_options, &option_index))) {

        if (c == -1)
            break;

        switch (c) {
        case 'l':
            read_liberty_file = true;
            liberty_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(liberty_file, optarg);
            break;
        case 'c':
            read_test_circuits_file = true;
            test_circuits_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(test_circuits_file, optarg);
            break;
        case 's':
            read_spef_file = true;
            spef_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spef_file, optarg);
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
    if (read_liberty_file) {

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
    if (read_spef_file) {

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
    if (read_test_circuits_file) {

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
