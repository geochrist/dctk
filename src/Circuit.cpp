// Circuit
//
// A circuit is defined as a input voltage source, driver instance,
// driver interconnect, load instance, and load interconnect
//

#include <string>
#include <iostream>
#include <src/Circuit.hpp>

namespace dctk {

Circuit::Circuit(const std::string& s) {
    this->_name = s;
}

Circuit& Circuit::set_input_voltage_source(const std::string& s) {
    this->_input_voltage_source = s;
    return *this;
}

Circuit& Circuit::set_driver(const std::string& s) {
    this->_driver = s;
    return *this;
}

Circuit& Circuit::set_driver_interconnect(const std::string& s) {
    this->_driver_interconnect = s;
    return *this;
}

Circuit& Circuit::set_load(const std::string& s) {
    this->_load = s;
    return *this;
}

Circuit& Circuit::set_load_interconnect(const std::string& s) {
    this->_load_interconnect = s;
    return *this;
}

void Circuit::dump() {
    std::cout << "Name = " << this->_name << std::endl;
    std::cout << "Voltage Source = " << this->_input_voltage_source << std::endl;
    std::cout << "Driver = " << this->_driver << std::endl;
    std::cout << "Driver Interconnect = " << this->_driver_interconnect << std::endl;
    std::cout << "Load = " << this->_load << std::endl;
    std::cout << "load_interconnect = " << this->_load_interconnect << std::endl;
    std::cout << "----------" << std::endl;
}

}


