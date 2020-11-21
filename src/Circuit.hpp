#ifndef DCTK_CIRCUIT_HPP_
#define DCTK_CIRCUIT_HPP_

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <parser-spef.hpp>
#include "CellLib.hpp"
#include "RCNet.hpp"

namespace dctk {

//
// Representation of a stage.
//
// A stage an interconnect (net) together with one or more driving cells, one or
// and one or more load cells. Limitations:
//   - driving cells are assumed to have only one input, and hence only one
//     (rise/fall) cell delay value.
//   - load cells are assumed to have only one input, and hence only one
//     (rise/fall) slew value.
// Based on the above delay(driver_cell, load_cell) is well-defined net delay.
// 
class Circuit {

public:

    // power node/net
    static std::string power;
    static float power_valuea;

    // ground node/net
    static std::string ground;

    Circuit(const std::string&);

    // maintenance APIs
    const std::string& get_name();

    // topology APIs

    // driver info
    Circuit& add_driver(std::string iname); // instance name
    Circuit& set_driver_celltype(std::string iname, std::string cell_type);
    // driver cells are assumed to be 1-input
    Circuit& set_input_waveform(std::string iname, std::string waveform);
                                
    std::vector<std::string> get_drivers();
    std::string get_driver_celltype(std::string iname);
    std::string get_input_waveform(std::string iname);
    
    // load info
    Circuit& add_load(std::string iname); // instance name
    Circuit& set_load_celltype(std::string iname, std::string cell_type);

    std::vector<std::string> get_loads();
    std::string get_load_celltype(std::string iname);

    // interconnect
    Circuit& set_pimodel_interconnect(float cnear, float res, float cfar);
    Circuit& set_pimodel_interconnect(const std::string& s);
        
    RCNet& get_interconnect();
    
    // misc routines
    bool is_positive_unate(CellLib& cl);

    // merge SPICE results in as golden
    void read_spice_results(const char*, const std::string&);
    
    // output methods
    void dump();

    void write_spice_voltages(std::fstream& fs, CellLib& cl);
    void write_spice_driver(std::fstream& fs, CellLib& cl);
    void write_spice_net(std::fstream& fs, spef::Spef& s);
    void write_spice_load(std::fstream& fs, CellLib& cl);
    void write_spice_load_parasitics(std::fstream& fs, CellLib& cl);
    void write_spice_commands(std::fstream& fs, CellLib& cl);
    void write_spice_deck(const std::string&, CellLib* cl, spef::Spef* s,
                          const char* spice_lib_name, const char* spice_models,
                          const char* simulator);
    
    void gen_yaml(YAML::Emitter& e);


    // misc values
    void set_power_rail_voltage(float f) {
        _power_rail_voltage = f;
    }
    const float get_power_rail_voltage() {
        return _power_rail_voltage;
    }

    // SPICE delay and slews
    void set_spice_rise_slew(float f) {
        _spice_rise_slew = f;
    }
    void set_spice_fall_slew(float f) {
        _spice_fall_slew = f;
    }
    void set_spice_rise_delay(float f) {
        _spice_rise_delay = f;
    }
    void set_spice_fall_delay(float f) {
        _spice_fall_delay = f;
    }

    float get_spice_rise_slew() {
        return _spice_rise_slew;
    }
    float get_spice_fall_slew() {
        return _spice_fall_slew;
    }
    float get_spice_rise_delay() {
        return _spice_rise_delay;
    }
   float get_spice_fall_delay() {
        return _spice_fall_delay;
    }

    // CCS delay and slews
    void set_ccs_rise_slew(float f) {
        _ccs_rise_slew = f;
    }
    void set_ccs_fall_slew(float f) {
        _ccs_fall_slew = f;
    }
    void set_ccs_rise_delay(float f) {
        _ccs_rise_delay = f;
    }
    void set_ccs_fall_delay(float f) {
        _ccs_fall_delay = f;
    }
    float get_ccs_rise_slew() {
        return _ccs_rise_slew;
    }
    float get_ccs_fall_slew() {
        return _ccs_fall_slew;
    }
    float get_ccs_rise_delay() {
        return _ccs_rise_delay;
    }
    float get_ccs_fall_delay() {
        return _ccs_fall_delay;
    }

    // simulation time
    void set_sim_time(float f) {
        _sim_time = f;
    }
    
    float get_sim_time() {
        return _sim_time;
    }

    void set_ngspice_measure () {
        _spice_measure = SpiceMeasure::ngspice;
    }

    void set_xyce_measure () {
        _spice_measure = SpiceMeasure::xyce;
    }

private:

    // TEMP(anton): implementation equivalent to old 1-input/1-load
    std::string _name;
    std::string _input_waveform;
    std::string _driver;
    std::string _driver_celltype;
    std::string _load;
    std::string _load_celltype; 

    RCNet _interconnect;

    float _power_rail_voltage;

    // spice/ccs delay and slew values in ps
    float _spice_rise_delay;
    float _spice_fall_delay;
    float _spice_rise_slew;
    float _spice_fall_slew;
    float _ccs_rise_delay;
    float _ccs_fall_delay;
    float _ccs_rise_slew;
    float _ccs_fall_slew;

    // in ps
    float _sim_time;

    // spice measurement styles
    enum class SpiceMeasure { ngspice, xyce} _spice_measure;

};

}

#endif
