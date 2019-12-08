// SPICE Lib Reader
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include "spice_lib_reader.hpp"
#include "CellLib.hpp"

int read_spice_lib(char *spice_lib_file_name, dctk::CellLib* cell_lib) {

    std::ifstream infile(spice_lib_file_name);

    if (!infile.good()) {
        std::cerr << "can't find " <<  spice_lib_file_name << '\n';
        return(1);
    }

    std::cout << "Processing spice lib " << spice_lib_file_name << std::endl;
    
    std::string line;
    while (std::getline(infile, line)) {

        // look for .SUBCKT in the line. Example:
        //    .SUBCKT SDFFR_X2 VDD VSS Q QN RN CK SI SE D 
        size_t position = line.find(".SUBCKT");
        if (position == 0) {
            // std::cout << "Found subckt:  " << line << std::endl;
            // break up into different fields

            std::istringstream iss(line);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                             std::istream_iterator<std::string>());
            
            // Second token is the cell name
            const std::string& cell_name = results[1];
            //std::cout << "  cell: " << cell_name << std::endl;

            // find cell in library
            dctk::Cell* cell = cell_lib->get_cell(cell_name);
            if (cell == NULL) {
                //std::cout << "no cell named " << cell_name << std::endl;
                continue;
            }

            // remaining tokens are the pin names
            // for ( int i=2; i < results.size(); i++) {

                // std::cout << "  Pin: " << results[i] << std::endl;
            // }

            // push pin names into Cell definition
            std::vector<std::string> pins (&results[2],&results[results.size()]);
            cell->set_pin_order(pins);
        }
    }
    infile.close();
    return(0);

}
