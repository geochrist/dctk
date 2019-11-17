// SPICE Lib Reader
#ifndef SPICE_LIB_READER_HPP_
#define SPICE_LIB_READER_HPP_

#include <vector>
#include "CellLib.hpp"

int read_spice_lib(char *filename, dctk::CellLib* cell_lib);

#endif
