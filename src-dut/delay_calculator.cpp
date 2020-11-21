#include <iostream>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "dctk.hpp"
#include "CellLib.hpp"
#include "Circuit.hpp"
#include <parser-spef.hpp>


// code to split strings
// from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

// From https://helloacm.com/cc-function-to-compute-the-bilinear-interpolation/
inline float 
BilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y) 
{
    const float x2x1 = x2 - x1;
    const float y2y1 = y2 - y1;
    const float x2x = x2 - x;
    const float y2y = y2 - y;
    const float yy1 = y - y1;
    const float xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

float lookup_table_value(const liberty_value_data* table, float slew, float load) {

    // first check that slew and load are within the table ranges
    unsigned int num_slews = table->dim_sizes[0];
    float min_slew = table->index_info[0][0];
    float max_slew = table->index_info[0][num_slews-1];   

    if ((slew < min_slew) || (slew > max_slew)) {
        std::cout << "Error: slew outside table ranges:  " << std::endl;
        std::cout << "slew = " << slew << "; min_slew = " << min_slew << "; max_slew = " << max_slew << std::endl;
        return 0.0;
    }
    
    unsigned int num_loads = table->dim_sizes[1];
    float min_load = table->index_info[1][0];
    float max_load = table->index_info[1][num_loads-1];   

    if ((load < min_load) || (load > max_load)) {
        std::cout << "Error: load outside table ranges:  " ;
        std::cout << "load = " << load << "; min_load = " << min_load << "; max_load = " << max_load << std::endl;
        return 0.0;
    }


    // x axis == slew
    // y axis == load

    float x2 = 0.0;
    
    unsigned int slew_index = 0;
    for ( ; slew_index < num_slews; slew_index++ ) {
        x2 = table->index_info[0][slew_index];
        if (x2 > slew) {
            break;
        }
    }
    slew_index--;
    float x1 = table->index_info[0][slew_index];
    
    float y2 = 0.0;
    
    unsigned int load_index = 0;
    for ( ; load_index < num_loads; load_index++ ) {
        y2 = table->index_info[1][load_index];
        if (y2 > load) {
            break;
        }
    }
    
    load_index--;
    float y1 = table->index_info[1][load_index];
        
    // now do the table lookup
    unsigned int index = num_slews * slew_index + load_index;
    const float q11 = table->values[index];

    index = num_slews * (slew_index + 1) + load_index;
    const float q21 = table->values[index];

    index = num_slews * slew_index + (load_index + 1);
    const float q12 = table->values[index];

    index = num_slews * (slew_index + 1) + (load_index + 1);
    const float q22 = table->values[index];

    float delay = BilinearInterpolation(q11, q12, q21, q22, x1, x2, y1, y2, slew, load);

    return delay;
}

// this is sort of slow ... how can we make it faster?
spef::Net* find_net(spef::Spef *spef, const std::string& name) {

    size_t numNets = spef->nets.size();
    for (size_t i = 0; i < numNets; i++) {
        if (spef->nets[i].name == name) {
            return &(spef->nets[i]);
        }

    }
    return nullptr;
}


void compute_delays_bilinear(dctk::CellLib* cell_lib, dctk::Circuit* circuit, spef::Spef* spef) {

    // get driver
    std::string driver = circuit->get_drivers()[0];

    // get slew in ps
    std::string waveform = circuit->get_input_waveform(driver);
    
    std::istringstream iss(waveform);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    char *end;
    float slew = strtod(results[1].c_str(), &end);

    // get load from SPEF
    const std::string& netname = circuit->get_name();
    spef::Net* net = find_net(spef, netname);
    float load = net->lcap; // in ff
    
    // driver cell
    std::string driver_celltype = circuit->get_driver_celltype(driver);
    dctk::Cell* cell = cell_lib->get_cell(driver_celltype);

    // get output pin
    dctk::CellPin* output_pin = cell->get_output_pin();

    // get input pin name from driver string
    std::string input_pin = split(driver, '/')[1];

    // find arc (second token is input pin)
    dctk::CellArc* arc = output_pin->find_arc(input_pin);

    // convert slew and load to library units
    float scale_to_ps = cell_lib->get_scale_to_ps();
    slew = slew / scale_to_ps;
    load = load / cell_lib->get_scale_to_ff();

    float rise_delay = lookup_table_value(arc->get_cell_rise_table(), slew, load);
    float fall_delay = lookup_table_value(arc->get_cell_fall_table(), slew, load);
    float rise_transition = lookup_table_value(arc->get_rise_transition_table(), slew, load);
    float fall_transition = lookup_table_value(arc->get_fall_transition_table(), slew, load);

    // save results
    circuit->set_ccs_rise_delay(rise_delay * scale_to_ps);
    circuit->set_ccs_fall_delay(fall_delay * scale_to_ps);
    circuit->set_ccs_rise_slew(rise_transition * scale_to_ps);
    circuit->set_ccs_fall_slew(fall_transition * scale_to_ps);

    return ;
}


int compute_delays(dctk::CellLib* cell_lib, dctk::CircuitPtrVec* circuitMgr, spef::Spef* spef) {

    std::cout << "Computing Delays for " << circuitMgr->size()
	      << " circuits." << std::endl;


    // Example on how to set return values for delay and slew
    int numCircuits = circuitMgr->size();
    for (int i = 0; i < numCircuits; i++) {
        compute_delays_bilinear(cell_lib, (*circuitMgr)[i], spef);
    }

    // allocate 100MB of space, to test memory measurement
    long size = 100*1000000;
    int* int_array = new int[size];
    int total = 0;
    memset(int_array, 0, size);
    for (int i = 0; i < size; i++) {
        total += int_array[i];
    }
    printf("Printing out a random number to disable optimizer: %d\n", total);
    
    // 2.3 seconds of delay
    std::this_thread::sleep_for(std::chrono::milliseconds(2300));

    delete[] int_array;
	
    return 0;

}
