#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>
#include "PiModel.hpp"

namespace dctk {

class Circuit {

public:

    Circuit(const std::string&);
    Circuit& set_input_voltage_source(const std::string&);
    Circuit& set_driver(const std::string&);
    Circuit& set_driver_interconnect(const std::string&);
    Circuit& set_load(const std::string&);
    Circuit& set_load_interconnect(const std::string&);
    void dump();

    // return values
    void set_slew(float f) { _slew = f; }
    void set_delay(float f) { _delay = f; }
    float get_slew(float f) { return _slew; }
    float get_delay(float f) { return _delay; }
    


private:

    std::string _name;
    std::string _input_voltage_source;
    std::string _driver;
    PiModel _driver_interconnect;
    std::string _load;
    PiModel _load_interconnect;

    // return values
    float _delay;
    float _slew;


};

}

#endif
