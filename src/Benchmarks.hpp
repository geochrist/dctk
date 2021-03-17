#ifndef DCTK_BENCHMARKS_HPP_
#define DCTK_BENCHMARKS_HPP_

#include <yaml-cpp/yaml.h>

namespace dctk {

class Benchmarks {

public:

    void dump_yaml(YAML::Emitter& emitter) {
        emitter << YAML::Key << "Statistics";
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "total_circuits" << YAML::Value << total_circuits ;
        emitter << YAML::Key << "valid_circuits" << YAML::Value << valid_circuits ;
        emitter << YAML::Key << "elapsed_time" << YAML::Value << elapsed_time ;
        emitter << YAML::Key << "cpu_time" << YAML::Value << cpu_time ;
        emitter << YAML::Key << "incremental_memory" << YAML::Value <<  incremental_memory ;
        emitter << YAML::Key << "cell_rms_delay_diff" << YAML::Value <<  cell_rms_delay_diff ;
        emitter << YAML::Key << "cell_rms_slew_diff" << YAML::Value <<  cell_rms_slew_diff ;
        emitter << YAML::Key << "net_rms_delay_diff" << YAML::Value <<  net_rms_delay_diff ;
        emitter << YAML::Key << "net_rms_slew_diff" << YAML::Value <<  net_rms_slew_diff ;
        emitter << YAML::Key << "cell_delay_outliers" << YAML::Value <<  cell_delay_outliers ;
        emitter << YAML::Key << "cell_slew_outliers" << YAML::Value <<  cell_slew_outliers ;
        emitter << YAML::Key << "net_delay_outliers" << YAML::Value <<  net_delay_outliers ;
        emitter << YAML::Key << "net_slew_outliers" << YAML::Value <<  net_slew_outliers ;
        emitter << YAML::Key << "cell_delay_pts" << YAML::Value <<  cell_delay_pts ;
        emitter << YAML::Key << "cell_slew_pts" << YAML::Value <<  cell_slew_pts ;
        emitter << YAML::Key << "net_delay_pts" << YAML::Value <<  net_delay_pts ;
        emitter << YAML::Key << "net_slew_pts" << YAML::Value <<  net_slew_pts ;
        emitter << YAML::EndMap;

    }

    // total/valid circuits
    unsigned total_circuits     = 0;
    unsigned valid_circuits     = 0;
    // in seconds
    float cpu_time              = 0.0;
    // in seconds
    float elapsed_time          = 0.0;
    // in MB
    float incremental_memory    = 0.0;
    // in percentage
    float cell_rms_delay_diff   = -1.0;
    float net_rms_delay_diff    = -1.0;
    // in percentage
    float cell_rms_slew_diff    = -1.0;
    float net_rms_slew_diff      = -1.0;    
    // points for delay accuracy
    float cell_delay_pts        = 0.0;
    float net_delay_pts         = 0.0;
    // points for slew accuracy
    float cell_slew_pts         = 0.0;
    float net_slew_pts          = 0.0;    

    // number of outliers
    unsigned int cell_delay_outliers = 0;
    unsigned int net_delay_outliers  = 0;    
    unsigned int cell_slew_outliers  = 0;
    unsigned int net_slew_outliers   = 0;    
};

}

#endif
