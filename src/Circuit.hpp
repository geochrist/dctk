#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>
#include <yaml-cpp/yaml.h>
#include "PiModel.hpp"

namespace dctk {

class Circuit {

public:

    Circuit(const std::string&);
    Circuit& set_input_voltage_source(const std::string&);

    // driver info
    Circuit& set_driver(const std::string&); // instance name
    Circuit& set_driver_celltype(const std::string&);
    Circuit& set_driver_interconnect(const std::string&);

    // receiver info
    Circuit& set_load(const std::string&); // instance name
    Circuit& set_load_celltype(const std::string&);
    Circuit& set_load_interconnect(const std::string&);

    void dump();

    // return values
    void set_slew(float f) { _slew = f; }
    void set_delay(float f) { _delay = f; }
    float get_slew(float f) { return _slew; }
    float get_delay(float f) { return _delay; }

    void gen_yaml(YAML::Emitter& e);


private:

    std::string _name;
    std::string _input_voltage_source;
    std::string _driver;
    std::string _driver_celltype;
    PiModel _driver_interconnect;
    std::string _load;
    std::string _load_celltype;
    PiModel _load_interconnect;

    // return values
    float _delay;
    float _slew;


};

}

#endif
