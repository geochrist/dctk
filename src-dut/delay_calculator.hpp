#ifndef DELAY_CALCULATOR_HPP_
#define DELAY_CALCULATOR_HPP_

#include "dctk.hpp"
#include <parser-spef.hpp>

int compute_delays(dctk::CellLib* cell_lib, dctk::CircuitPtrVec* circuitMgr, spef::Spef *spef);

#endif
