// CellLib
//
// A CellLib is a library of Cells
//

#include <string>
#include <iostream>
#include <map>

#include "CellPin.hpp"

namespace dctk {

CellPin::CellPin(const std::string& s) {
    _name = s;

}

void CellPin::add_arc(CellArc* a) {
    _arcs.push_back(a);
}

CellArc* CellPin::find_arc(const std::string& input_pin) {

    std::vector<CellArc*>::iterator iter = _arcs.begin();
    std::vector<CellArc*>::iterator iterEnd = _arcs.end();

    // return first arc for which related_pin == input_pin
    for ( ; iter != iterEnd; iter++) {
        CellArc* arc = *iter;
        if (arc->get_related_pin()->get_name() == input_pin) {
            return arc;
        }
    }

    // arc doesn't exist
    return nullptr;
}

void CellPin::dump() {
    std::cout << "    Pin Name = " << this->_name << std::endl;

    // print out all the cells that exist

    std::vector<dctk::CellArc*>::iterator it;

    for (it = this->_arcs.begin(); it != this->_arcs.end(); it++) {
        (*it)->dump();
    }

}

CellArc* CellPin::get_random_arc() {
    // find a random arc attached to that output pin

    if (_arcs.size() == 0) {
        return (nullptr);
    }
    size_t index = rand() % _arcs.size();
    return _arcs[index];

}
}

