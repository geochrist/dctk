// CellLib
//
// A CellLib is a library of Cells
//

#include <string>
#include <iostream>
#include <src/CellLib.hpp>

namespace dctk {

CellLib::CellLib(const std::string& s) {
    this->_name = s;

}

void CellLib::insert(const std::string& s, dctk::Cell* cell) {

    this->_cellMap[s] = cell ;

}


void CellLib::dump() {
    std::cout << "Cell Lib Name = " << this->_name << std::endl;

    // print out all the cells that exist
    std::map<std::string, Cell*>::iterator it;

    for (it = this->_cellMap.begin(); it != this->_cellMap.end(); it++) {
        it->second->dump();
    }

}

}


