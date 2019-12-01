// Cell
//
// Cell contains all the modeling data necessary for delay calculation
//

#include <string>
#include <iostream>
#include <src/Cell.hpp>

namespace dctk {

Cell::Cell(const std::string& s) {
    this->_name = s;

}

void Cell::dump() {
    std::cout << "Cell Name = " << this->_name << std::endl;

    std::map<std::string, dctk::CellPin*>::iterator it;
    for (it = this->_pinMap.begin(); it != this->_pinMap.end(); it++) {
        //it->second->dump();
    }


}

void Cell::add_pin(const std::string& s, dctk::CellPin* pin) {
    this->_pinMap[s] = pin ;
}

bool Cell::fixup_pins_in_arcs() {

    bool retval = true;

    // iterate through each pin
    std::map<std::string, CellPin*>::iterator it_pinmap = _pinMap.begin();

    for (; it_pinmap != _pinMap.end(); it_pinmap++) {

        // iterate through each arc attached to the pin
        std::vector<CellArc*>::iterator it_arcs = it_pinmap->second->_arcs.begin();
        for ( ; it_arcs != it_pinmap->second->_arcs.end(); it_arcs++) {

            const std::string& related_pin_str = (*it_arcs)->_related_pin_str;

            // look for pin with this name
            std::map<std::string, CellPin*>::iterator it_pin;
            if ( (it_pin = _pinMap.find(related_pin_str)) != _pinMap.end()) {
                // update pin pointer
                (*it_arcs)-> update_related_pin(it_pin->second);
            } else {
                // print error
                std::cout << "Cannot find pin reference for " << related_pin_str << std::endl;
                retval = false;

            }

        }

    }
    return retval;

}

CellPin* Cell::get_output_pin() {
    // returns first output pin found

    std::map<std::string, dctk::CellPin*>::iterator pin_iter = _pinMap.begin();

    for ( ; pin_iter != _pinMap.end(); pin_iter++) {
        if (pin_iter->second->get_direction() == "output") {
            return pin_iter->second;
        }
    }

    return nullptr;

}

CellPin* Cell::get_random_input_pin() {

    // collects all input pins, and then randomly chooses one

    std::map<std::string, dctk::CellPin*>::iterator pin_iter = _pinMap.begin();
    std::vector<dctk::CellPin*> input_pins;

    for ( ; pin_iter != _pinMap.end(); pin_iter++) {
        if (pin_iter->second->get_direction() == "input") {
            input_pins.push_back(pin_iter->second);
        }
    }

    int num_input_pins = input_pins.size();
    if (input_pins.size() == 0) {
        return nullptr;
    }

    int random_input_pin = rand() % num_input_pins;
    return input_pins[random_input_pin];

}

}

