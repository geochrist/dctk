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

    const std::string& get_name();
    const std::string& get_input_waveform();

    // driver info
    Circuit& set_driver(const std::string&); // instance name
    Circuit& set_driver_celltype(const std::string&);

    const std::string& get_driver();
    const std::string& get_driver_celltype();


    // receiver info
    Circuit& set_load(const std::string&); // instance name
    Circuit& set_load_celltype(const std::string&);
    Circuit& set_load_interconnect(const std::string&);

    const std::string& get_load();
    const std::string& get_load_celltype();
    const PiModel& get_load_interconnect();
    
    // misc routines
    bool is_positive_unate(CellLib& cl);

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

    std::string _name;
    std::string _input_waveform;
    std::string _driver;
    std::string _driver_celltype;
    std::string _load;
    std::string _load_celltype; 
    PiModel _load_interconnect;

    float _power_rail_voltage;

    // spice/ccs delay and slew values
    float _spice_rise_delay;
    float _spice_fall_delay;
    float _spice_rise_slew;
    float _spice_fall_slew;
    float _ccs_rise_delay;
    float _ccs_fall_delay;
    float _ccs_rise_slew;
    float _ccs_fall_slew;

    float _sim_time;

    // spice measurement styles
    enum class SpiceMeasure { ngspice, xyce} _spice_measure;

};

}

#endif
