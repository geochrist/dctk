#ifndef DCTK_CELLLIB_HPP_
#define DCTK_CELLLIB_HPP_

#include <cstring>
#include <string>
#include <map>
#include <iostream>
#include "Cell.hpp"

namespace dctk {

class CellLib {

public:


    CellLib(const std::string&);

    void insert(const std::string&, dctk::Cell*);

    void dump();

    Cell* get_cell(const std::string& s) {
        std::map<std::string, Cell*>::iterator item = _cellMap.find(s);
        if (item != _cellMap.end()) {
            return item->second;
        } else {
            return nullptr;
        }
    }

    const std::string& get_name() {
        return _name;
    }
    
    Cell* get_random_cell() {
        std::map<std::string, Cell*>::iterator item = _cellMap.begin();
        std::advance( item, rand() % _cellMap.size() );
        return item->second;
    }

    // setters
    void set_time_unit(const std::string& s) {
        _time_unit = s ;

        if (!strcmp(s.c_str(), "1ns")) {
            _scale_to_ps = 1000.0;
        }

        if (!strcmp(s.c_str(), "1ps")) {
            _scale_to_ps = 1.0;
        }
            
    }
    void set_leakage_power_unit(const std::string& s) {
        _leakage_power_unit = s ;
    }
    void set_voltage_unit(const std::string& s) {
        _voltage_unit = s ;
    }
    void set_current_unit(const std::string& s) {
        _current_unit = s ;
    }
    void set_pulling_resistance_unit(const std::string& s) {
        _pulling_resistance_unit = s ;
    }
    void set_capacitive_load_unit(const std::string& s) {
        _capacitive_load_unit = s ;
        
        if (!strcmp(s.c_str(), "1ff")) {
            _scale_to_ff = 1.0;
        }
        if (!strcmp(s.c_str(), "1pf")) {
            _scale_to_ff = 1000.0;
        }
    }

    void set_nom_process(const float& f) {
         _nom_process = f ;
    }
    void set_nom_temperature(const float& f) {
         _nom_temperature = f ;
    }
    void set_nom_voltage(const float& f) {
         _nom_voltage = f ;
    }

    void set_slew_lower_threshold_pct_fall(const float& f) {
         _slew_lower_threshold_pct_fall = f ;
    }
    void set_slew_lower_threshold_pct_rise(const float& f) {
         _slew_lower_threshold_pct_rise = f ;
    }
    void set_slew_upper_threshold_pct_fall(const float& f) {
         _slew_upper_threshold_pct_fall = f ;
    }
    void set_slew_upper_threshold_pct_rise(const float& f) {
         _slew_upper_threshold_pct_rise = f ;
    }
    void set_slew_derate_from_library(const float& f) {
         _slew_derate_from_library = f ;
    }
    void set_input_threshold_pct_fall(const float& f) {
         _input_threshold_pct_fall = f ;
    }
    void set_input_threshold_pct_rise(const float& f) {
         _input_threshold_pct_rise = f ;
    }
    void set_output_threshold_pct_fall(const float& f) {
         _output_threshold_pct_fall = f ;
    }
    void set_output_threshold_pct_rise(const float& f) {
         _output_threshold_pct_rise = f ;
    }

    // getters
    const std::string& get_time_unit() {
        return _time_unit ;
    }
    const std::string& get_leakage_power_unit() {
        return _leakage_power_unit ;
    }
    const std::string& get_voltage_unit() {
        return _voltage_unit ;
    }
    const std::string& get_current_unit() {
        return _current_unit ;
    }
    const std::string& get_pulling_resistance_unit() {
        return _pulling_resistance_unit ;
    }
    const std::string& get_capacitive_load_unit() {
        return _capacitive_load_unit ;
    }
    
    const float& get_nom_process() {
        return _nom_process;
    }
    const float& get_nom_temperature() {
        return _nom_temperature;
    }
    const float& get_nom_voltage() {
        return _nom_voltage;
    }

    const float& get_slew_lower_threshold_pct_fall() {
        return _slew_lower_threshold_pct_fall;
    }
    const float& get_slew_lower_threshold_pct_rise() {
        return _slew_lower_threshold_pct_rise;
    }
    const float& get_slew_upper_threshold_pct_fall() {
        return _slew_upper_threshold_pct_fall;
    }
    const float& get_slew_upper_threshold_pct_rise() {
        return _slew_upper_threshold_pct_rise;
    }
    const float& get_slew_derate_from_library() {
        return _slew_derate_from_library;
    }
    const float& get_input_threshold_pct_fall() {
        return _input_threshold_pct_fall;
    }
    const float& get_input_threshold_pct_rise() {
        return _input_threshold_pct_rise;
    }
    const float& get_output_threshold_pct_fall() {
        return _output_threshold_pct_fall;
    }
    const float& get_output_threshold_pct_rise() {
        return _output_threshold_pct_rise;
    }

    // provide access to voltage map, for faster access
    // TODO: isolate interface from caller
    std::map<std::string, float>& get_voltage_map() {
        return _voltage_map;
    }

    const float& get_scale_to_ps() {
        return _scale_to_ps;
    }

    const float& get_scale_to_ff() {
        return _scale_to_ff;
    }
    

private:


    std::string _name;

    std::string _time_unit;
    std::string _leakage_power_unit;
    std::string _voltage_unit;
    std::string _current_unit;
    std::string _pulling_resistance_unit;
    std::string _capacitive_load_unit;

    // scale values to convert Liberty time units to ps
    float _scale_to_ps;

    // scale values to convert Liberty cap units to ff
    float _scale_to_ff;

    std::map<std::string, Cell*> _cellMap;
    std::map<std::string,float> _voltage_map;

    float _nom_process;
    float _nom_temperature;
    float _nom_voltage;

    float _slew_lower_threshold_pct_fall ;
    float _slew_lower_threshold_pct_rise ;
    float _slew_upper_threshold_pct_fall ;
    float _slew_upper_threshold_pct_rise ;
    float _slew_derate_from_library ;
    float _input_threshold_pct_fall ;
    float _input_threshold_pct_rise ;
    float _output_threshold_pct_fall ;
    float _output_threshold_pct_rise ;
};

}

#endif
