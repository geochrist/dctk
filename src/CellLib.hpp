#ifndef DCTK_CELLLIB_HPP_
#define DCTK_CELLLIB_HPP_

#include <string>
#include <map>
#include "Cell.hpp"

namespace dctk {

class CellLib {

public:

    CellLib(const std::string&);

    void insert(const std::string&, dctk::Cell*);

    void dump();


private:

    std::string _name;
    std::map<std::string, Cell*> _cellMap;

};

}

#endif
