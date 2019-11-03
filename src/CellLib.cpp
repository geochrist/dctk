// CellLib
//
// A CellLib is a library of Cells
//

#include <string>
#include <iostream>
#include <src/CellLib.hpp>

namespace dctk {

CellLib::CellLib(const std::string& s) {
    this->_name = s;

    this->_nom_process = 0.0;
    this->_nom_temperature = 25.0;
    this->_nom_voltage = 0.0;

    this->_slew_lower_threshold_pct_fall = 10.0;
    this->_slew_lower_threshold_pct_rise = 10.0;
    this->_slew_upper_threshold_pct_fall = 90.0;
    this->_slew_upper_threshold_pct_rise = 90.0;
    this->_slew_derate_from_library = 1.0 ;
    this->_input_threshold_pct_fall = 50.0 ;
    this->_input_threshold_pct_rise = 50.0 ;
    this->_output_threshold_pct_fall = 50.0 ;
    this->_output_threshold_pct_rise = 50.0 ;


}

void CellLib::insert(const std::string& s, dctk::Cell* cell) {

    this->_cellMap[s] = cell ;

}


void CellLib::dump() {
    std::cout << "Cell Lib Name = " << this->_name << std::endl;

    // dump library level properties
    // dump library level properties
    std::cout << "  time_unit = " << this->get_time_unit() << std::endl;
    std::cout << "  leakage_power_unit = " << this->get_leakage_power_unit() << std::endl;
    std::cout << "  leakage_voltage_unit = " << this->get_voltage_unit() << std::endl;
    std::cout << "  current_unit = " << this->get_current_unit() << std::endl;
    std::cout << "  pulling_resistance_unit = " << this->get_pulling_resistance_unit() << std::endl;
    std::cout << "  capacitive_load_unit = " << this->get_capacitive_load_unit() << std::endl;

    std::cout << "  nom_process = " << this->get_nom_process() << std::endl;
    std::cout << "  nom_temperature = " << this->get_nom_temperature() << std::endl;
    std::cout << "  nom_voltage = " << this->get_nom_voltage() << std::endl;

    std::cout << "  slew_lower_threshold_pct_fall = " << this->get_slew_lower_threshold_pct_fall() << std::endl;
    std::cout << "  slew_lower_threshold_pct_rise = " << this->get_slew_lower_threshold_pct_rise() << std::endl;
    std::cout << "  slew_upper_threshold_pct_fall = " << this->get_slew_upper_threshold_pct_fall() << std::endl;
    std::cout << "  slew_upper_threshold_pct_rise = " << this->get_slew_upper_threshold_pct_rise() << std::endl;

    std::cout << "  slew_derate_from_library = " << this->get_slew_derate_from_library() << std::endl;

    std::cout << "  input_threshold_pct_fall = " << this->get_input_threshold_pct_fall() << std::endl;
    std::cout << "  input_threshold_pct_rise = " << this->get_input_threshold_pct_rise() << std::endl;
    std::cout << "  output_threshold_pct_fall = " << this->get_output_threshold_pct_fall() << std::endl;
    std::cout << "  output_threshold_pct_rise = " << this->get_output_threshold_pct_rise() << std::endl;

    // print out all the cells that exist
    std::map<std::string, Cell*>::iterator it;

    for (it = this->_cellMap.begin(); it != this->_cellMap.end(); it++) {
        // it->second->dump();
    }

}

}


