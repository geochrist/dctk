#ifndef DCTK_CELLLIB_HPP_
#define DCTK_CELLLIB_HPP_

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

    Cell* get_random_cell() {
	std::map<std::string, Cell*>::iterator item = _cellMap.begin();
	std::advance( item, rand() % _cellMap.size() );
	return item->second;
    }

private:

    std::string _name;
    std::map<std::string, Cell*> _cellMap;

};

}

#endif
