#include <iostream>

#include "dctk.hpp"
#include "CellLib.hpp"
#include "Circuit.hpp"
#include <parser-spef.hpp>

int compute_delays(dctk::CellLib* cell_lib, dctk::CircuitPtrVec* circuitMgr, spef::Spef* spef) {

    std::cout << "Computing Delays for " << circuitMgr->size()
	      << " circuits." << std::endl;


    // Example on how to set return values for delay and slew
    int numCircuits = circuitMgr->size();
    for (int i=0; i < numCircuits; i++) {
	
	(*circuitMgr)[i]->set_ccs_delay(i);
	(*circuitMgr)[i]->set_ccs_slew(i*0.2);

    }
	
    return 0;

}
