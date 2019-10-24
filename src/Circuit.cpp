// Circuit
//
// A circuit is defined as a input voltage source, driver instance,
// driver interconnect, load instance, and load interconnect
//

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <src/Circuit.hpp>

namespace dctk {

Circuit::Circuit(const std::string& s) {
    _name = s;
    _delay = 0.0;
    _slew = 0.0;
}

Circuit& Circuit::set_input_voltage_source(const std::string& s) {
    this->_input_voltage_source = s;
    return *this;
}

Circuit& Circuit::set_driver(const std::string& s) {
    this->_driver = s;
    return *this;
}
Circuit& Circuit::set_driver_celltype(const std::string& s) {
    this->_driver_celltype = s;
    return *this;
}

Circuit& Circuit::set_driver_interconnect(const std::string& s) {

    // split string into 3 tokens and assign to cnear, res, and cfar
 
    std::istringstream iss(s);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				     std::istream_iterator<std::string>());

    // there should be exactly 3 tokens
    this->_driver_interconnect.set_cnear(stof(results[0]));
    this->_driver_interconnect.set_res(stof(results[1]));
    this->_driver_interconnect.set_cfar(stof(results[2]));
    return *this;
}

Circuit& Circuit::set_load(const std::string& s) {

    this->_load = s;
    return *this;
}

Circuit& Circuit::set_load_celltype(const std::string& s) {

    this->_load_celltype = s;
    return *this;
}

Circuit& Circuit::set_load_interconnect(const std::string& s) {

    std::istringstream iss(s);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				     std::istream_iterator<std::string>());

    // there should be exactly 3 tokens
    this->_load_interconnect.set_cnear(stof(results[0]));
    this->_load_interconnect.set_res(stof(results[1]));
    this->_load_interconnect.set_cfar(stof(results[2]));
    return *this;
}

void Circuit::dump() {
    std::cout << "Name = " << this->_name << std::endl;
    std::cout << "Voltage Source = " << this->_input_voltage_source << std::endl;
    std::cout << "Driver = " << this->_driver << std::endl;
    std::cout << "Driver Interconnect = " << this->_driver_interconnect.get_cnear()
	      << " " << this->_driver_interconnect.get_res()
	      << " " << this->_driver_interconnect.get_cfar()
	      << std::endl;
    std::cout << "Load = " << this->_load << std::endl;
    std::cout << "load_interconnect = " << this->_load_interconnect.get_cnear()
	      << " " << this->_load_interconnect.get_res()
	      << " " << this->_load_interconnect.get_cfar()
	      << std::endl;
    std::cout << "delay = " << this->_delay << std::endl;
    std::cout << "slew = " << this->_slew << std::endl;
    std::cout << "----------" << std::endl;
}

void Circuit::gen_yaml(YAML::Emitter& emitter) {

    emitter << YAML::BeginMap;

    emitter << YAML::Key << "name" << YAML::Value << this->_name;

    emitter << YAML::Key << "driver" << YAML::Value << this->_driver;

    emitter << YAML::Key << "driver_celltype" << YAML::Value << this->_driver_celltype;

    emitter << YAML::Key << "voltage_source" << YAML::Value << this->_input_voltage_source;

    // build string for driver PI model
    std::stringstream ss1;
    ss1 << this->_driver_interconnect.get_cnear()
	<< " " << this->_driver_interconnect.get_res()
	<< " " << this->_driver_interconnect.get_cfar();

    emitter << YAML::Key << "driver_interconnect" << YAML::Value << ss1.str();

    emitter << YAML::Key << "load" << YAML::Value << this->_load;

    emitter << YAML::Key << "load_celltype" << YAML::Value << this->_load_celltype;

    std::stringstream ss2;
    ss2 << this->_load_interconnect.get_cnear()
	<< " " << this->_load_interconnect.get_res()
	<< " " << this->_load_interconnect.get_cfar();

    emitter << YAML::Key << "load_interconnect" << YAML::Value << ss2.str();

    emitter << YAML::Key << "delay" << YAML::Value << this->_delay;
    emitter << YAML::Key << "slew" << YAML::Value << this->_slew;
    
    emitter << YAML::EndMap;

}
}
