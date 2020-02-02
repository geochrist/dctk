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
        rms_delay_diff = -1.0;
        rms_slew_diff = -1.0;
        delay_pts = 0.0;
        slew_pts = 0.0;
        
    }
    
    void dump_yaml(YAML::Emitter& emitter) {
        emitter << YAML::Key << "Statistics";
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "elapsed_time" << YAML::Value << elapsed_time ;
        emitter << YAML::Key << "cpu_time" << YAML::Value << cpu_time ;
        emitter << YAML::Key << "incremental_memory" << YAML::Value <<  incremental_memory ;
        emitter << YAML::Key << "rms_delay_diff" << YAML::Value <<  rms_delay_diff ;
        emitter << YAML::Key << "rms_slew_diff" << YAML::Value <<  rms_slew_diff ;
        emitter << YAML::Key << "delay_pts" << YAML::Value <<  delay_pts ;
        emitter << YAML::Key << "slew_pts" << YAML::Value <<  slew_pts ;
        emitter << YAML::EndMap;

    }
    
    // in seconds
    float cpu_time;
    // in seconds
    float elapsed_time;
    // in MB
    float incremental_memory;
    // in percentage
    float rms_delay_diff;
    // in percentage
    float rms_slew_diff;
    // points for delay accuracy
    float delay_pts;
    // points for slew accuracy
    float slew_pts;

};

}

#endif
