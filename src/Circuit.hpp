#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>

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


private:

    std::string _name;
    std::string _input_voltage_source;
    std::string _driver;
    std::string _driver_interconnect;
    std::string _load;
    std::string _load_interconnect;

};

}

#endif
