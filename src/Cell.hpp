#ifndef DCTK_CELL_HPP_
#define DCTK_CELL_HPP_

#include <string>
#include <map>
#include "CellPin.hpp"

namespace dctk {

class Cell {

public:

    Cell(const std::string&);

    void add_pin(const std::string&, dctk::CellPin*);
    bool fixup_pins_in_arcs();

    // setters
    void set_pin_order(std::vector<std::string> vec) {
        _pin_order = vec;
    }
    
    // getters
    std::string& get_name() {
        return _name;
    };
    dctk::CellPin* get_output_pin();
    dctk::CellPin* get_random_input_pin();

    const std::vector<std::string>& get_pin_order() {
        return _pin_order;
    }
        

    // print output
    void dump();


private:

    std::string _name;
    std::map<std::string, dctk::CellPin*> _pinMap;
    std::vector<std::string> _pin_order;

};

}

#endif
