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
#include <ot/parser-spef/parser-spef.hpp>

// dctk definitions
#include "dctk.hpp"

// Liberty reader
#include "liberty_reader.hpp"

// Verilog reader
#include "ot/verilog/verilog.hpp"

// SDC reader
#include "ot/sdc/sdc.hpp"


// Benchmarks object definition
#include "Benchmarks.hpp"

//
// Usage:
//
// partition_timing_graph [options]
//
//   --liberty LIBERTY    Liberty model
//   --spef SPEF          SPEF file
//   --sdc  SDC           SDC file
//   --verilog VERILOG    Verilog file

int
main(int argc, char **argv)
{
    int c;

    // Liberty
    char* liberty_file = nullptr;
    dctk::CellLib* cell_lib = nullptr;
    int read_lib_retval = 0;

    // Verilog
    char* verilog_file = nullptr;
    ot::vlog::Module verilog;
    
    // Constraints
    char* constraints_file = nullptr;
    ot::sdc::SDC constraints;
    
    // SPEF
    char* spef_file = nullptr;
    spef::Spef* spef = nullptr;

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
        {"verilog", required_argument, 0, 'v'},
        {"sdc", required_argument, 0, 'c'},
        {0,         0,                 0,  0 }
    };

    while  ((c = getopt_long(argc, argv, "l:s:", long_options, &option_index))) {

        if (c == -1)
            break;

        switch (c) {
        case 'l':
            liberty_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(liberty_file, optarg);
            break;
        case 's':
            spef_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(spef_file, optarg);
            break;
        case 'v':
            verilog_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(verilog_file, optarg);
            break;
        case 'c':
            constraints_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(constraints_file, optarg);
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
        if (read_lib_retval != 0) {
            printf("Error %d during Liberty processing.  Exiting.", read_lib_retval);
            exit(1);
        }
        cell_lib->dump();
    }

    // Read Verilog
    if (verilog_file) {

        printf("Reading Verilog file %s\n", verilog_file);
        verilog = ot::vlog::read_verilog(verilog_file);
        // TODO error checking?
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

    // Read Spef
    if (constraints_file) {

        printf("Reading SDC file %s\n", constraints_file);
        constraints.read(constraints_file);
        // TODO error checking?
    }

    // Run partitioninng
    // benchmarking before computation
    getrusage(RUSAGE_SELF, &rusage_before);
    auto elapsed_t1 = std::chrono::steady_clock::now();

    // computation
    // gen_partition_retval = gen_partitions(cell_lib, spef);
    int gen_partition_retval = 0;
    if (gen_partition_retval != 0) {
        printf("Error %d generating partitions.  Exiting.", gen_partition_retval);
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

    exit(EXIT_SUCCESS);
}
