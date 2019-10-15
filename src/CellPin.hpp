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

    std::string get_name() {
        return _name;
    }

    void add_arc(CellArc* arc);

    void dump();


private:

    friend class Cell;
    std::string _name;
    std::vector<CellArc*> _arcs;

};

}

#endif