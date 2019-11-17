// Circuit
//
// A circuit is defined as a input voltage source, driver instance,
// driver interconnect, load instance, and load interconnect
//

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <src/Circuit.hpp>

namespace dctk {

Circuit::Circuit(const std::string& s) {
    _name = s;
    _spice_delay = 0.0;
    _spice_slew = 0.0;
    _ccs_delay = 0.0;
    _ccs_slew = 0.0;
}

Circuit& Circuit::set_input_waveform(const std::string& s) {
    this->_input_waveform = s;
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
    std::cout << "Input Waveform = " << this->_input_waveform << std::endl;
    std::cout << "Driver = " << this->_driver << std::endl;
    std::cout << "Driver Celltype = " << this->_driver_celltype << std::endl;
    std::cout << "Load = " << this->_load << std::endl;
    std::cout << "load_interconnect = " << this->_load_interconnect.get_cnear()
              << " " << this->_load_interconnect.get_res()
              << " " << this->_load_interconnect.get_cfar()
              << std::endl;
    std::cout << "spice delay = " << this->_spice_delay << std::endl;
    std::cout << "spice slew = " << this->_spice_slew << std::endl;
    std::cout << "ccs delay = " << this->_ccs_delay << std::endl;
    std::cout << "ccs slew = " << this->_ccs_slew << std::endl;
    std::cout << "----------" << std::endl;
}

void Circuit::gen_yaml(YAML::Emitter& emitter) {

    emitter << YAML::BeginMap;

    emitter << YAML::Key << "name" << YAML::Value << this->_name;

    emitter << YAML::Key << "driver" << YAML::Value << this->_driver;

    emitter << YAML::Key << "driver_celltype" << YAML::Value << this->_driver_celltype;

    emitter << YAML::Key << "input_waveform" << YAML::Value << this->_input_waveform;

    emitter << YAML::Key << "load" << YAML::Value << this->_load;

    emitter << YAML::Key << "load_celltype" << YAML::Value << this->_load_celltype;

    std::stringstream ss2;
    ss2 << this->_load_interconnect.get_cnear()
        << " " << this->_load_interconnect.get_res()
        << " " << this->_load_interconnect.get_cfar();

    emitter << YAML::Key << "load_interconnect" << YAML::Value << ss2.str();

    emitter << YAML::Key << "spice_delay" << YAML::Value << this->_spice_delay;
    emitter << YAML::Key << "spice_slew" << YAML::Value << this->_spice_slew;
    emitter << YAML::Key << "ccs_delay" << YAML::Value << this->_ccs_delay;
    emitter << YAML::Key << "ccs_slew" << YAML::Value << this->_ccs_slew;

    emitter << YAML::EndMap;

}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void Circuit::write_spice_voltages(std::fstream& fs, CellLib& cellLib) {

    fs << "* Constant Voltage Sources" << std::endl;

    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    std::map<std::string, float>::const_iterator iter = voltage_map.begin();
    std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();
    for ( ; iter != iterEnd; iter++) {
        const std::string& rail = iter->first;
        float voltage = iter->second;
        fs << "V" << rail << " " << rail << " 0 " << voltage << std::endl;
    }

}    

void Circuit::write_spice_driver(std::fstream& fs, CellLib& cellLib) {

    // get driver celltype
    Cell* driver_cell = cellLib.get_cell(_driver_celltype);

    // _driver = I1/A/Z
    std::vector<std::string> driver_info = split(_driver, '/');

    // setup for voltage rail identification
    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    const std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();

    //
    // figure out connectivity of instance pins
    //
    std::vector<std::string> pins;

    // print out pins, in order
    const std::string& inst = driver_info[0];
    std::vector<std::string> cellpins = driver_cell->get_pin_order();
    for (int i=0; i < cellpins.size(); i++) {
        // check to see if pin is a voltage rail
        if (voltage_map.find(cellpins[i]) != iterEnd) {
            pins.push_back(cellpins[i]);
        } else {
            pins.push_back(inst + ":" + cellpins[i]);
        }
    }
        
    //
    // write out driver instance
    //
    fs << "* Driver" << std::endl;

    // instance
    fs << "X" << driver_info[0];

    // pins
    for (int i=0; i< pins.size(); i++) {
        fs << " " << pins[i];
    }
    // celltype
    fs << " " << _driver_celltype << std::endl;
}

void Circuit::write_spice_net(std::fstream& fs, spef::Spef& spef) {
    fs << "* NET" << std::endl;

    // find SPEF infor
    // this is very inefficient due to SPEF being stored as a loop.
    // TODO:  build map for faster indexing

    for(const auto &n : spef.nets) {
        if (n.name != _name) {
            continue;
        }
        
        unsigned int index = 0;
        // we've found the net!
        // caps
        for(const auto& c : n.caps){
            auto& [node1, node2, value] = c;
            if (node2 == ""){
                // ground coupling caps
                fs << "C" << index << ' ' << node1 << " VSS " << value << '\n';
            } else {
                fs << "C" << index << ' ' << node1 << ' ' << node2 << ' ' << value << '\n';
            }
            index++;
        }

        // res
        index = 0;
        for(const auto& r : n.ress){
            auto& [node1, node2, value] = r;
            fs << "R" << index << ' ' << node1 << ' ' << node2 << ' ' << value << '\n';
            index++;
        }

    }        
        
}


void Circuit::write_spice_load(std::fstream& fs, CellLib& cellLib) {

    // get load celltype
    Cell* load_cell = cellLib.get_cell(_load_celltype);

    // _driver = I1/A/Z
    std::vector<std::string> load_info = split(_load, '/');

    //
    // figure out connectivity of instance pins
    //
    std::vector<std::string> pins;

    // setup for voltage rail identification
    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    const std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();

    // print out pins, in order
    const std::string& inst = load_info[0];
    std::vector<std::string> cellpins = load_cell->get_pin_order();
    for (int i=0; i < cellpins.size(); i++) {
        // check to see if pin is a voltage rail
        if (voltage_map.find(cellpins[i]) != iterEnd) {
            pins.push_back(cellpins[i]);
        } else {
            pins.push_back(inst + ":" + cellpins[i]);
        }
    }
        
    //
    // write out load instance
    //
    fs << "* Load" << std::endl;
    
    // instance
    fs << "X" << load_info[0];

    // pins
    for (int i=0; i< pins.size(); i++) {
        fs << " " << pins[i];
    }
    // celltype
    fs << " " << _load_celltype << std::endl;

        
}


void Circuit::write_spice_load_parasitics(std::fstream& fs, CellLib& cellLib) {

    // _load = I2/CK/Q
    std::vector<std::string> load_tokens = split(_load, '/');
    const std::string& inst = load_tokens[0];
    const std::string& output_pin = inst + ':' + load_tokens[2];
        
    //
    // write out load's parasitics
    //
    fs << "* Load parasitics" << std::endl;
    fs << "Cnear " << output_pin << " 0 " << _load_interconnect.get_cnear() << std::endl;

    // write out res and cfar only if they are non-zero
    // (most of time, load parasitics may be pure capacitance with only cnear)
    if ((_load_interconnect.get_res() != 0.0) || (_load_interconnect.get_cfar() != 0.0)) {
        fs << "Rres " << output_pin << " far_node " << _load_interconnect.get_res() << std::endl;
        fs << "Cfar far_node 0 " << _load_interconnect.get_cfar() << std::endl;
    }

}

void Circuit::write_spice_deck(const std::string& dirname, CellLib* cellLib, spef::Spef* spef,
                               const char* spice_lib_name, const char* spice_models) {

    std::fstream fs;
    std::string filename = dirname + "/" + _name + ".sp";
    fs.open(filename.c_str(), std::fstream::out);

    // title
    fs << "* SPICE circuit for " << _name << std::endl;

    // write out spice lib
    fs << ".include " << spice_lib_name << std::endl;

    // write out spice models
    fs << ".include " << spice_models << std::endl;

    // write out voltage source
    this->write_spice_voltages(fs, *cellLib);

    // write out driver
    this->write_spice_driver(fs, *cellLib);
    
    // write out net
    this->write_spice_net(fs, *spef);
        
    // write out receiver
    this->write_spice_load(fs, *cellLib);

    // write out load's parasitics
    this->write_spice_load_parasitics(fs, *cellLib);

    // end statement
    fs << ".end" << std::endl;

    // TODO: include models
    fs.close();
}

}
