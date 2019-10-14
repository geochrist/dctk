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

void CellPin::dump() {
    std::cout << "    Pin Name = " << this->_name << std::endl;

    // print out all the cells that exist

    std::vector<dctk::CellArc*>::iterator it;

    for (it = this->_arcs.begin(); it != this->_arcs.end(); it++) {
        (*it)->dump();
    }

}

}


