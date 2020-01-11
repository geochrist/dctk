#ifndef DCTK_CELLPIN_HPP_
#define DCTK_CELLPIN_HPP_

#include <string>
#include <vector>
#include "CellArc.hpp"

namespace dctk {

class CellArc;

class CellPin {

public:

    CellPin(const std::string&);

    const std::string& get_name() {
        return _name;
    }

    void set_direction(const std::string& d) {
        _direction = d;
    }
    void set_rise_capacitance(float f) {
        _rise_capacitance = f;
    }
    void set_rise_capacitance_range_min(float f) {
        _rise_capacitance_range_min = f;
    }
    void set_rise_capacitance_range_max(float f) {
        _rise_capacitance_range_max = f;
    }

    void set_fall_capacitance(float f) {
        _fall_capacitance = f;
    }
    void set_fall_capacitance_range_min(float f) {
        _fall_capacitance_range_min = f;
    }
    void set_fall_capacitance_range_max(float f) {
        _fall_capacitance_range_max = f;
    }



    const std::string& get_direction() {
        return _direction;
    }

    void add_arc(CellArc* arc);

    void dump();

    CellArc* find_arc(const std::string& input_pin);

    CellArc* get_random_arc();


private:

    friend class Cell;
    std::string _name;
    std::string _direction;
    std::vector<CellArc*> _arcs;

    float _rise_capacitance;
    float _rise_capacitance_range_min;
    float _rise_capacitance_range_max;
    float _fall_capacitance;
    float _fall_capacitance_range_min;
    float _fall_capacitance_range_max;
    
};

}

#endif
