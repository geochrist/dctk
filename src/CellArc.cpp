// CellLib
//
// A CellLib is a library of Cells
//

#include <string>
#include <iostream>
#include <map>

#include "CellArc.hpp"
#include "si2dr_liberty.h"

namespace dctk {

CellArc::CellArc(CellPin* pin, std::string& related_pin_str) {
    _pin = pin;
    _related_pin_str = related_pin_str;

    // NLDM tables
    _cell_rise_table = nullptr;
    _cell_fall_table = nullptr;
    _rise_transition_table = nullptr;
    _fall_transition_table = nullptr;

    // Receiver Capacitance tables
    _receiver_capacitance1_fall_table = nullptr;
    _receiver_capacitance1_rise_table = nullptr;
    _receiver_capacitance2_fall_table = nullptr;
    _receiver_capacitance2_rise_table = nullptr;

}

CellArc::~CellArc() {

    // destruct NLDM data
    liberty_destroy_value_data(_cell_rise_table);
    liberty_destroy_value_data(_cell_fall_table);
    liberty_destroy_value_data(_rise_transition_table);
    liberty_destroy_value_data(_fall_transition_table);

    // destruct Receiver Capacitance tables
    liberty_destroy_value_data(_receiver_capacitance1_fall_table);
    liberty_destroy_value_data(_receiver_capacitance1_rise_table);
    liberty_destroy_value_data(_receiver_capacitance2_fall_table);
    liberty_destroy_value_data(_receiver_capacitance2_rise_table);

    // destruct each of the CCS tables
    int len = _output_current_rise_tables.size();
    for(int i = 0; i < len; i++) {
        liberty_destroy_value_data(_output_current_rise_tables[i]);
    }

    len = _output_current_fall_tables.size();
    for(int i = 0; i < len; i++) {
        liberty_destroy_value_data(_output_current_fall_tables[i]);
    }

}


void CellArc::add_current_rise_table(liberty_value_data* d) {
    _output_current_rise_tables.push_back(d);
}

void CellArc::add_current_fall_table(liberty_value_data* d) {
    _output_current_fall_tables.push_back(d);
}

void dump_table(const liberty_value_data* table, const std::string& table_name) {

    // don't dump anything if we got a nullptr.
    if (table == nullptr) {
        return;
    }

    std::cout << "    Table " << table_name << " has the following data:" << std::endl;

    int num_dimensions = table -> dimensions;
    int total_element_count = 1;

    for (int d = 0; d < num_dimensions; d++) {

        // first print out the index values
        int dim_size = table->dim_sizes[d];
        LONG_DOUBLE *index_values = table->index_info[d];

        std::cout << "    index_" << d+1 << " values: ";
        for (int i = 0; i<dim_size; i++) {
            std::cout << " " << index_values[i];
        }
        std::cout << std::endl;

        total_element_count = total_element_count * dim_size;
    }

    // now dump whole array of values
    std::cout << "    values: " ;
    for (int i = 0; i < total_element_count; i++) {
        std::cout << " " << table->values[i];
    }
    std::cout << std::endl;

}

void CellArc::dump() {
    //std::cout << "    Arc with pin = " << _pin->get_name() << " & related pin = " << _related_pin_str << std::endl;
    std::cout << "    Arc with pin = " << _pin->get_name() << " & related pin = " << _related_pin->get_name() << std::endl;

    std::cout << "    timing_type = " << _timing_type_str << std::endl;
    std::cout << "    timing_sense = " << _timing_sense_str << std::endl;

    // dump each of the NLDM tables
    dump_table(_cell_rise_table, std::string("cell_rise"));
    dump_table(_rise_transition_table, std::string("rise_transition"));
    dump_table(_cell_fall_table, std::string("cell_fall"));
    dump_table(_fall_transition_table, std::string("fall_transition"));

    dump_table(_receiver_capacitance1_fall_table, std::string("receiver_capacitance1_fall"));
    dump_table(_receiver_capacitance1_rise_table, std::string("receiver_capacitance1_rise"));
    dump_table(_receiver_capacitance2_fall_table, std::string("receiver_capacitance2_fall"));
    dump_table(_receiver_capacitance2_rise_table, std::string("receiver_capacitance2_rise"));


    // dump each of the CCS tables
    int len = _output_current_rise_tables.size();
    for(int i = 0; i < len; i++) {
        const std::string table_name = "output current rise";
        dump_table(_output_current_rise_tables[i], table_name);
    }

    len = _output_current_fall_tables.size();
    for(int i = 0; i < len; i++) {
        const std::string table_name = "output current fall";
        dump_table(_output_current_fall_tables[i], table_name);
    }

}

}
