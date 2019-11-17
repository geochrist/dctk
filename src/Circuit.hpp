#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>
#include <yaml-cpp/yaml.h>
#include "PiModel.hpp"
#include "CellLib.hpp"
#include <parser-spef.hpp>

namespace dctk {

class Circuit {

public:

    // power node/net
    static std::string power;
    static float power_valuea;

    // ground node/net
    static std::string ground;

    
    Circuit(const std::string&);
    Circuit& set_input_waveform(const std::string&);

    // driver info
    Circuit& set_driver(const std::string&); // instance name
    Circuit& set_driver_celltype(const std::string&);

    // receiver info
    Circuit& set_load(const std::string&); // instance name
    Circuit& set_load_celltype(const std::string&);
    Circuit& set_load_interconnect(const std::string&);

    // output methods
    void dump();

    void write_spice_voltages(std::fstream& fs, CellLib& cl);
    void write_spice_driver(std::fstream& fs, CellLib& cl);
    void write_spice_net(std::fstream& fs, spef::Spef& s);
    void write_spice_load(std::fstream& fs, CellLib& cl);
    void write_spice_load_parasitics(std::fstream& fs, CellLib& cl);
    void write_spice_deck(const std::string&, CellLib* cl, spef::Spef* s,
                          const char* spice_lib_name, const char* spice_models);
    
    void gen_yaml(YAML::Emitter& e);

    // SPICE delay and slews
    void set_spice_slew(float f) {
        _spice_slew = f;
    }
    void set_spice_delay(float f) {
        _spice_delay = f;
    }

    float get_spice_slew() {
        return _spice_slew;
    }
    float get_spice_delay() {
        return _spice_delay;
    }

    // CCS delay and slews
    void set_ccs_slew(float f) {
        _ccs_slew = f;
    }
    void set_ccs_delay(float f) {
        _ccs_delay = f;
    }
    float get_ccs_slew() {
        return _ccs_slew;
    }
    float get_ccs_delay() {
        return _ccs_delay;
    }

private:

    std::string _name;
    std::string _input_waveform;
    std::string _driver;
    std::string _driver_celltype;
    std::string _load;
    std::string _load_celltype;
    PiModel _load_interconnect;

    // spice/ccs delay and slew values
    float _spice_delay;
    float _spice_slew;
    float _ccs_delay;
    float _ccs_slew;


};

}

#endif
