#include <iostream>
#include <thread>
#include <stdio.h>
#include <string.h>

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
	
	(*circuitMgr)[i]->set_ccs_rise_delay(i);
	(*circuitMgr)[i]->set_ccs_fall_delay(i);
	(*circuitMgr)[i]->set_ccs_rise_slew(i*0.2);
	(*circuitMgr)[i]->set_ccs_fall_slew(i*0.2);

    }

    // allocate 100MB of space, to test memory measurement
    long size = 100*1000000;
    int* int_array = new int[size];
    int total = 0;
    memset(int_array, 0, size);
    for (int i = 0; i < size; i++) {
        total += int_array[i];
    }
    printf("total value is %d\n", total);
    
    // 2.3 seconds of delay
    std::this_thread::sleep_for(std::chrono::milliseconds(2300));

    delete[] int_array;
	
    return 0;

}
