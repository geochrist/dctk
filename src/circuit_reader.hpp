// Circuit Reader
#ifndef CIRCUIT_READER_HPP_
#define CIRCUIT_READER_HPP_

#include <vector>
#include "Circuit.hpp"

int read_circuits(char *filename, std::vector<dctk::Circuit*>* circuit_lib);

#endif
