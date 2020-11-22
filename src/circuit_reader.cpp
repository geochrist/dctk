// Circuits Reader
//#include <experimental/filesystem>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "Circuit.hpp"
#include <string>

#include "dctk.hpp"
#include "circuit_reader.hpp"
#include "Circuit.hpp"
#include "Benchmarks.hpp"

int read_circuits(char *circuit_file_name, dctk::CircuitPtrVec *circuitMgr, dctk::Benchmarks* benchmarks) {

    std::ifstream infile(circuit_file_name);
    if (!infile.good()) {
        std::cerr << "can't find " <<  circuit_file_name << '\n';
        return(1);
    }

    std::cout << "Reading circuits from " << circuit_file_name << std::endl;

    // load the circuit list file
    const YAML::Node& config = YAML::LoadFile(circuit_file_name);

    // check for valid circuit config file
    if (config["Circuits"]) {
        const std::size_t& num_circuits = config["Circuits"].size();
        std::cout << "Processing " << num_circuits << " circuits." << std::endl;
    } else {
        std::cout << "Could not find 'Circuits' key in " << circuit_file_name << std::endl;
        return(1);
    }

    const YAML::Node& circuits_yaml = config["Circuits"];

    for (std::size_t i = 0 ; i < circuits_yaml.size(); i++) {

        // Transfer from yaml file into CircuitMgr object
        const YAML::Node& circuit = circuits_yaml[i];


        const std::string& name = circuit["name"].as<std::string>();

        dctk::Circuit* cir = new dctk::Circuit(name);

        const std::string& str_vs=circuit["input_waveform"].as<std::string>();
        cir->set_input_waveform(str_vs);

        const std::string& str_driver=circuit["driver"].as<std::string>();
        cir->set_driver(str_driver);

        const std::string& str_driver_celltype=circuit["driver_celltype"].as<std::string>();
        cir->set_driver_celltype(str_driver_celltype);

        const std::string& str_load=circuit["load"].as<std::string>();
        cir->set_load(str_load);

        const std::string& str_load_celltype=circuit["load_celltype"].as<std::string>();
        cir->set_load_celltype(str_load_celltype);

        const std::string& str_load_interconnect=circuit["load_interconnect"].as<std::string>();
        cir->set_load_interconnect(str_load_interconnect);

        const std::string& str_unused_loads=circuit["unused_loads"].as<std::string>();
        cir->set_unused_loads(str_unused_loads);

        const std::string& str_spice_rise_delay=circuit["spice_rise_delay"].as<std::string>();
        cir->set_spice_rise_delay(atof(str_spice_rise_delay.c_str()));

        const std::string& str_spice_fall_delay=circuit["spice_fall_delay"].as<std::string>();
        cir->set_spice_fall_delay(atof(str_spice_fall_delay.c_str()));

        const std::string& str_spice_rise_slew=circuit["spice_rise_slew"].as<std::string>();
        cir->set_spice_rise_slew(atof(str_spice_rise_slew.c_str()));

        const std::string& str_spice_fall_slew=circuit["spice_fall_slew"].as<std::string>();
        cir->set_spice_fall_slew(atof(str_spice_fall_slew.c_str()));

        const std::string& str_ccs_rise_delay=circuit["ccs_rise_delay"].as<std::string>();
        cir->set_ccs_rise_delay(atof(str_ccs_rise_delay.c_str()));

        const std::string& str_ccs_fall_delay=circuit["ccs_fall_delay"].as<std::string>();
        cir->set_ccs_fall_delay(atof(str_ccs_fall_delay.c_str()));

        const std::string& str_ccs_rise_slew=circuit["ccs_rise_slew"].as<std::string>();
        cir->set_ccs_rise_slew(atof(str_ccs_rise_slew.c_str()));

        const std::string& str_ccs_fall_slew=circuit["ccs_fall_slew"].as<std::string>();
        cir->set_ccs_fall_slew(atof(str_ccs_fall_slew.c_str()));

        circuitMgr->push_back(cir);
    }

    if (config["Statistics"]) {

        const YAML::Node& stats_yaml = config["Statistics"];
    
        // cpu time
        if (stats_yaml["cpu_time"]) {
            benchmarks->cpu_time = stats_yaml["cpu_time"].as<float>();
        }
        if (stats_yaml["elapsed_time"]) {
            benchmarks->elapsed_time = stats_yaml["elapsed_time"].as<float>();
        }
        if (stats_yaml["incremental_memory"]) {
            benchmarks->incremental_memory = stats_yaml["incremental_memory"].as<float>();
        }
        if (stats_yaml["rms_delay_diff"]) {
            benchmarks->rms_delay_diff = stats_yaml["rms_delay_diff"].as<float>();
        }
        if (stats_yaml["rms_slew_diff"]) {
            benchmarks->rms_slew_diff = stats_yaml["rms_slew_diff"].as<float>();
        }
        
    }

    return 0;

}

