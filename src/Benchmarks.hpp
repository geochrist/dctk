#ifndef DCTK_BENCHMARKS_HPP_
#define DCTK_BENCHMARKS_HPP_

#include <yaml-cpp/yaml.h>

namespace dctk {

class Benchmarks {

public:

    Benchmarks() {
        cpu_time = 0.0;
        elapsed_time = 0.0;
        incremental_memory = 0.0;
    }
    
    void dump_yaml(YAML::Emitter& emitter) {
        emitter << YAML::Key << "Statistics";
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "elapsed_time" << YAML::Value << elapsed_time ;
        emitter << YAML::Key << "cpu_time" << YAML::Value << cpu_time ;
        emitter << YAML::Key << "incremental_memory" << YAML::Value <<  incremental_memory ;
        emitter << YAML::EndMap;

    }
    
    // in seconds
    float cpu_time;
    // in seconds
    float elapsed_time;
    // in MB
    float incremental_memory;

};

}

#endif
