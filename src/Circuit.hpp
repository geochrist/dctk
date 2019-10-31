#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>
#include <yaml-cpp/yaml.h>
#include "PiModel.hpp"

namespace dctk {

class Circuit {

public:

    Circuit(const std::string&);
    Circuit& set_input_waveform(const std::string&);

    // driver info
    Circuit& set_driver(const std::string&); // instance name
    Circuit& set_driver_celltype(const std::string&);

    // receiver info
    Circuit& set_load(const std::string&); // instance name
    Circuit& set_load_celltype(const std::string&);
    Circuit& set_load_interconnect(const std::string&);

    void dump();

    // return values
    void set_spice_slew(float f) {
        _spice_slew = f;
    }
    void set_spice_delay(float f) {
        _spice_delay = f;
    }
    float get_spice_slew(float f) {
        return _spice_slew;
    }
    float get_spice_delay(float f) {
        return _spice_delay;
    }

    void set_ccs_slew(float f) {
        _ccs_slew = f;
    }
    void set_ccs_delay(float f) {
        _ccs_delay = f;
    }
    float get_ccs_slew(float f) {
        return _ccs_slew;
    }
    float get_ccs_delay(float f) {
        return _ccs_delay;
    }

    void gen_yaml(YAML::Emitter& e);


private:

    std::string _name;
    std::string _input_waveform;
    std::string _driver;
    std::string _driver_celltype;
    std::string _load;
    std::string _load_celltype;
    PiModel _load_interconnect;

    // return values
    float _spice_delay;
    float _spice_slew;
    float _ccs_delay;
    float _ccs_slew;


};

}

#endif
