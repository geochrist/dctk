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
    void dump();


private:

    std::string _name;
    std::map<std::string, dctk::CellPin*> _pinMap;

};

}

#endif
