//
// Circuit is a representation of a stage.
//
// A stage an interconnect (net) together with one or more driving cells, one or
// and one or more load cells. Limitations:
//   - driving cells are assumed to have only one input, and hence only one
//     (rise/fall) cell delay value.
//   - load cells are assumed to have only one input, and hence only one
//     (rise/fall) slew value.
// Based on the above delay(driver_cell, load_cell) is well-defined net delay.
// 

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <regex>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <src/Circuit.hpp>
#include <src/AsuDriverWave.hpp>

namespace dctk {

Circuit::Circuit(const std::string& s) {
    _name = s;
    _spice_rise_delay = 0.0;
    _spice_fall_delay = 0.0;
    _spice_rise_slew = 0.0;
    _spice_fall_slew = 0.0;
    _ccs_rise_delay = 0.0;
    _ccs_fall_delay = 0.0;
    _ccs_rise_slew = 0.0;
    _ccs_fall_slew = 0.0;
    _spice_measure = SpiceMeasure::ngspice;
}

const std::string& Circuit::get_name() {
   return _name;
}

// driver info
Circuit& Circuit::add_driver(std::string iname) {
    this->_driver = iname;
    return *this;
}
    
Circuit& Circuit::set_driver_celltype(std::string iname, std::string cell_type) {
    if (_driver == iname) {
        this->_driver_celltype = cell_type;
    }
    return *this;
}

Circuit& Circuit::set_input_waveform(std::string iname, std::string waveform) {
    if (_driver == iname) {
        this->_input_waveform = waveform;
    }
    return *this;
}

std::vector<std::string> Circuit::get_drivers() {
    return { _driver };
}
std::string Circuit::get_driver_celltype(std::string iname) {
    return (_driver == iname) ? _driver_celltype : "";
}
std::string Circuit::get_input_waveform(std::string iname) {
    return (_driver == iname) ? _input_waveform : "";
}

// receiver
Circuit& Circuit::add_load(std::string iname) {
    this->_load = iname;
    return *this;
}
Circuit& Circuit::set_load_celltype(std::string iname, std::string cell_type) {
    if (_load == iname) {
        this->_load_celltype = cell_type;
    }
    return *this;
}
std::vector<std::string> Circuit::get_loads() {
    return { _load };
}
std::string Circuit::get_load_celltype(std::string iname) {
    return (_load == iname) ? _load_celltype : "";
}

// interconnect
RCNet& Circuit::get_interconnect() {

    return _interconnect;
}
Circuit& Circuit::set_pimodel_interconnect(float cnear, float res, float cfar) {

    _interconnect.clear();
    _interconnect.set_name("net1");
    std::string driver_node = _interconnect.add_driver_node(_driver);
    std::string load_node = _interconnect.add_load_node(_load);
    _interconnect
        .set_cap(driver_node, cnear)
        .set_cap(load_node, cfar)
        .set_res(driver_node, load_node, res);
    return *this;

}
Circuit& Circuit::set_pimodel_interconnect(const std::string& s) {

    std::istringstream iss(s);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    set_pimodel_interconnect(stof(results[0]),
                             stof(results[1]),
                             stof(results[2]));
    return *this;
}
std::string Circuit::get_pimodel_interconnect() {
    assert(_interconnect.get_driver_nodes().size() == 1);
    assert(_interconnect.get_load_nodes().size() == 1);
    assert(_interconnect.get_internal_nodes().size() == 0);
    std::string dr = _interconnect.get_driver_nodes()[0];
    std::string ld = _interconnect.get_load_nodes()[0];
    float cnear = _interconnect.get_cap(dr);
    float cfar = _interconnect.get_cap(ld);
    float res = _interconnect.get_res(dr, ld);
    return std::to_string(cnear) + " " + std::to_string(res)
        + " " + std::to_string(cfar);
}
   

void Circuit::dump() {
    std::cout << "Name = " << this->_name << std::endl;
    std::cout << "Input Waveform = " << this->_input_waveform << std::endl;
    std::cout << "Driver = " << this->_driver << std::endl;
    std::cout << "Driver Celltype = " << this->_driver_celltype << std::endl;
    std::cout << "Load = " << this->_load << std::endl;
    std::cout << "Load Celltype = " << this->_load_celltype << std::endl;
    std::cout << "Interconnect = " << this->_interconnect.to_string() << std::endl;
    std::cout << "spice rise delay = " << this->_spice_rise_delay << std::endl;
    std::cout << "spice fall delay = " << this->_spice_fall_delay << std::endl;
    std::cout << "spice rise slew = " << this->_spice_rise_slew << std::endl;
    std::cout << "spice fall slew = " << this->_spice_fall_slew << std::endl;
    std::cout << "ccs rise delay = " << this->_ccs_rise_delay << std::endl;
    std::cout << "ccs fall delay = " << this->_ccs_fall_delay << std::endl;
    std::cout << "ccs rise slew = " << this->_ccs_rise_slew << std::endl;
    std::cout << "ccs fall slew = " << this->_ccs_fall_slew << std::endl;
    std::cout << "----------" << std::endl;
}

void Circuit::gen_yaml(YAML::Emitter& emitter) {

    emitter << YAML::BeginMap;

    emitter << YAML::Key << "name" << YAML::Value << this->_name;

    emitter << YAML::Key << "driver" << YAML::Value << this->_driver;

    emitter << YAML::Key << "driver_celltype" << YAML::Value << this->_driver_celltype;

    emitter << YAML::Key << "input_waveform" << YAML::Value << this->_input_waveform;

    emitter << YAML::Key << "load" << YAML::Value << this->_load;

    emitter << YAML::Key << "load_celltype" << YAML::Value << this->_load_celltype;

    emitter << YAML::Key << "load_interconnect" << YAML::Value << this->get_pimodel_interconnect();

    emitter << YAML::Key << "spice_rise_delay" << YAML::Value << this->_spice_rise_delay;
    emitter << YAML::Key << "spice_fall_delay" << YAML::Value << this->_spice_fall_delay;
    emitter << YAML::Key << "spice_rise_slew" << YAML::Value << this->_spice_rise_slew;
    emitter << YAML::Key << "spice_fall_slew" << YAML::Value << this->_spice_fall_slew;
    emitter << YAML::Key << "ccs_rise_delay" << YAML::Value << this->_ccs_rise_delay;
    emitter << YAML::Key << "ccs_fall_delay" << YAML::Value << this->_ccs_fall_delay;
    emitter << YAML::Key << "ccs_rise_slew" << YAML::Value << this->_ccs_rise_slew;
    emitter << YAML::Key << "ccs_fall_slew" << YAML::Value << this->_ccs_fall_slew;

    emitter << YAML::EndMap;

}

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

void Circuit::write_spice_voltages(std::fstream& fs, CellLib& cellLib) {

    fs << "* Constant Voltage Sources" << std::endl;

    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    std::map<std::string, float>::const_iterator iter = voltage_map.begin();
    std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();
    for ( ; iter != iterEnd; iter++) {
        const std::string& rail = iter->first;
        float voltage = iter->second;
        fs << "V" << rail << "_rise " << rail << "_rise 0 " << voltage << std::endl;
        fs << "V" << rail << "_fall " << rail << "_fall 0 " << voltage << std::endl;
        // identify voltage rail by the one that is not at 0.0V
        if (voltage != 0.0) {
            set_power_rail_voltage(voltage);
        }
    }

    // figure out waveform
    
    std::istringstream iss(_input_waveform);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    // there should be exactly 2 tokens
    const std::string& type = results[0];

    // TODO: convert to strtod() for consistency
    float tramp = atof(results[1].c_str());

    // set power_rail
    const float power_rail_voltage = get_power_rail_voltage();
    const float ground_rail_voltage = 0.0;

    // we use a pulse
    std::vector<std::string> driver_info = split(_driver, '/');

    bool invert = not is_positive_unate(cellLib);

    if (type == "asu_exp") {

        float offset = tramp * 2.0;
        // offset translates the waveform left/right.          
        auto w = AsuDriverWave(tramp, offset, power_rail_voltage,
                               cellLib.get_slew_upper_threshold_pct_rise(),
                               cellLib.get_slew_lower_threshold_pct_rise());
        
        // duplicating same trip points as characterization
        auto voltages = w.voltages();
        auto times = w.times();

        // now write out the voltages for rising portion
        float initial_rail_voltage;
        float final_rail_voltage;
        if (invert) {
            initial_rail_voltage = power_rail_voltage;
            final_rail_voltage = ground_rail_voltage;
        } else {
            initial_rail_voltage = ground_rail_voltage;
            final_rail_voltage = power_rail_voltage;
        }
            

        // stimulus
        // build up PWL; times in ps
        // voltage source
        fs << "Vinput_rise";
       
        // nodes
        fs << " " + driver_info[0] + ':' + driver_info[1] << "_rise 0";
        // TODO:  pull ramp time from circuits.yaml
        fs << " pwl(0p " << initial_rail_voltage << "v" ;

        size_t n = voltages.size();
        float total_sim_time = 0.0;
        for (size_t i = 0; i < n; i++) {
            float voltage;
            if (invert) {
                voltage = std::max((float)(power_rail_voltage - voltages[i]),(float)0.0);
            } else {
                voltage = voltages[i];
            }

            fs << " " << times[i] << "p " << voltage << "v";
            total_sim_time = times[i] + tramp * 2.0 + 500.0;
        }
        fs << ")\n" ;

        // now work on falling measurement
        // voltage source
        fs << "Vinput_fall";
       
        // nodes
        fs << " " + driver_info[0] + ':' + driver_info[1] << "_fall 0";
        // TODO:  pull ramp time from circuits.yaml
        fs << " pwl(0p " << final_rail_voltage << "v" ;

        for (size_t i = 0; i < n; i++) {
            float voltage;
            if (invert) {
                voltage = voltages[i];
            } else {
                voltage = std::max((float)(power_rail_voltage - voltages[i]),(float)0.0);
            }
            fs << " " << times[i] << "p " << voltage << "v";
        }
        fs << ")\n" ;


        // add additional delay to make sure we capture trailing waveform
        set_sim_time(total_sim_time);
        return;
    }
    
    if (type != "ramp") {
        
        std::cout << "Error:  Found " << type << " waveform, but only ramp and asu_exp supported. Assumping ramp." << std::endl;
    }
    

    // TODO: scale according to library thresholds
    float scale = 100.0 / (cellLib.get_slew_upper_threshold_pct_rise() -
                           cellLib.get_slew_lower_threshold_pct_rise());
    float tramp_rise = tramp * scale;
        
    scale = 100.0 / (cellLib.get_slew_upper_threshold_pct_fall() -
                     cellLib.get_slew_lower_threshold_pct_fall());
    float tramp_fall = tramp * scale;
        
    fs << "* stimulus" << std::endl;

    // stimulus rise
    std::string vstim_str = "Vinput_rise";

    // build up PWL; times in ps
    const float tstart = 100.0;
    const float tpulse = 1900.0;

    float initial_rail_voltage;
    float pulse_rail_voltage;
    
    // set initial and pulse voltages based on unateness
    if (invert) {
        initial_rail_voltage = power_rail_voltage;
        pulse_rail_voltage = 0.0;
    } else {
        initial_rail_voltage = 0.0;
        pulse_rail_voltage = power_rail_voltage;
    }

    // voltage source
    fs << "Vinput_rise";
       
    // nodes
    fs << " " + driver_info[0] + ':' + driver_info[1] << "_rise 0";
    // TODO:  pull ramp time from circuits.yaml
    fs << " pwl(0p " << initial_rail_voltage << "v" ;
    // beginning of rise
    fs << " " << tstart << "p " << initial_rail_voltage << "v";
    // end of rise
    fs << " " << tstart + tramp_rise << "p " << pulse_rail_voltage << "v" ;
    // beginning of fall
    fs << " " << tstart + tramp_rise + tpulse << "p " << pulse_rail_voltage << "v" ;
    // end of fall
    fs << " " << tstart + tramp_rise + tpulse + tramp_fall << "p " << initial_rail_voltage << "v";
    // end of simulation
    fs << " " << tstart + tramp_rise + tpulse + tramp_fall + tpulse << "p " << initial_rail_voltage << "v";
    fs << ")" << std::endl;


    // stimulus fall
    vstim_str = "Vinput_fall";

    // voltage source
    fs << "Vinput_fall";
       
    // nodes
    fs << " " + driver_info[0] + ':' + driver_info[1] << "_fall 0";
    // TODO:  pull ramp time from circuits.yaml
    fs << " pwl(0p " << pulse_rail_voltage << "v" ;
    // beginning of rise
    fs << " " << tstart << "p " << pulse_rail_voltage << "v";
    // end of rise
    fs << " " << tstart + tramp_rise << "p " << initial_rail_voltage << "v" ;
    // beginning of fall
    fs << " " << tstart + tramp_rise + tpulse << "p " << initial_rail_voltage << "v" ;
    // end of fall
    fs << " " << tstart + tramp_rise + tpulse + tramp_fall << "p " << pulse_rail_voltage << "v";
    // end of simulation
    fs << " " << tstart + tramp_rise + tpulse + tramp_fall + tpulse << "p " << pulse_rail_voltage << "v";
    fs << ")" << std::endl;


    // store the total simulation time for the .tran statement
    // set_sim_time(tstart + tramp_rise + tpulse + tramp_fall + tpulse);
    set_sim_time(tstart + tramp_rise + tpulse);

    return;

}

void Circuit::write_spice_driver(std::fstream& fs, CellLib& cellLib) {

    // get driver celltype
    Cell* driver_cell = cellLib.get_cell(_driver_celltype);

    // _driver = I1/A/Z
    std::vector<std::string> driver_info = split(_driver, '/');

    // setup for voltage rail identification
    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    const std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();

    //
    // figure out connectivity of instance pins
    //
    std::vector<std::string> pins;

    // print out pins, in order
    const std::string& inst = driver_info[0];
    std::vector<std::string> cellpins = driver_cell->get_pin_order();
    for (unsigned int i=0; i < cellpins.size(); i++) {
        // check to see if pin is a voltage rail
        if (voltage_map.find(cellpins[i]) != iterEnd) {
            pins.push_back(cellpins[i]);
        } else {
            pins.push_back(inst + ":" + cellpins[i]);
        }
    }
        
    //
    // write out driver instance
    //
    fs << "* Driver" << std::endl;

    // instance rise
    fs << "X" << driver_info[0] << "_rise";

    // pins
    for (unsigned int i=0; i< pins.size(); i++) {
        fs << " " << pins[i] << "_rise";
    }
    // celltype
    fs << " " << _driver_celltype << std::endl;


    // instance fall
    fs << "X" << driver_info[0] << "_fall";

    // pins
    for (unsigned int i=0; i< pins.size(); i++) {
        fs << " " << pins[i] << "_fall";
    }
    // celltype
    fs << " " << _driver_celltype << std::endl;
}

void Circuit::write_spice_net(std::fstream& fs, spef::Spef& spef) {
    fs << "* NET" << std::endl;

    // find SPEF infor
    // this is very inefficient due to SPEF being stored as a loop.
    // TODO:  build map for faster indexing

    for(const auto &n : spef.nets) {
        if (n.name != _name) {
            continue;
        }
        
        unsigned int index = 0;
        // we've found the net!
        // caps
        for(const auto& c : n.caps){
            auto& [node1, node2, value] = c;
            if (node2 == ""){
                // TODO -- figure out units from SPEF.  Support only FF right now.

                // ground coupling caps
                fs << "C" << index << "_rise " << node1 << "_rise VSS_rise " << value << "FF \n";
                fs << "C" << index << "_fall " << node1 << "_fall VSS_fall " << value << "FF \n";
            } else {
                fs << "C" << index << "_rise " << node1 << "_rise " << node2 << "_rise " << value << "FF \n";
                fs << "C" << index << "_fall " << node1 << "_fall " << node2 << "_fall " << value << "FF \n";
            }
            index++;
        }

        // res
        index = 0;
        for(const auto& r : n.ress){
            auto& [node1, node2, value] = r;
            // TODO -- figure out units from SPEF.  Support only ohms right now.

            fs << "R" << index << "_rise " << node1 << "_rise " << node2 << "_rise " << value << '\n';
            fs << "R" << index << "_fall " << node1 << "_fall " << node2 << "_fall " << value << '\n';
            index++;
        }

    }        
        
}


void Circuit::write_spice_load(std::fstream& fs, CellLib& cellLib) {

    // get load celltype
    Cell* load_cell = cellLib.get_cell(_load_celltype);

    // _driver = I1/A/Z
    std::vector<std::string> load_info = split(_load, '/');

    //
    // figure out connectivity of instance pins
    //
    std::vector<std::string> pins;

    // setup for voltage rail identification
    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    const std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();

    // print out pins, in order
    const std::string& inst = load_info[0];
    std::vector<std::string> cellpins = load_cell->get_pin_order();
    for (unsigned int i=0; i < cellpins.size(); i++) {
        // check to see if pin is a voltage rail
        if (voltage_map.find(cellpins[i]) != iterEnd) {
            pins.push_back(cellpins[i]);
        } else {
            pins.push_back(inst + ":" + cellpins[i]);
        }
    }
        
    //
    // write out load instances
    //
    fs << "* Load" << std::endl;
    
    // instance rise
    fs << "X" << load_info[0] << "_rise";

    // pins rise
    for (unsigned int i=0; i< pins.size(); i++) {
        fs << " " << pins[i] << "_rise";
    }
    // celltype
    fs << " " << _load_celltype << std::endl;

    // instance fall
    fs << "X" << load_info[0] << "_fall";

    // pins rise
    for (unsigned int i=0; i< pins.size(); i++) {
        fs << " " << pins[i] << "_fall";
    }
    // celltype
    fs << " " << _load_celltype << std::endl;

}

void Circuit::write_spice_load_parasitics(std::fstream& fs, CellLib& cellLib) {
    
    // TODO(anton): rewrite to accomodate RCNet interconnect
    
    // _load = I2/CK/Q
    std::vector<std::string> load_tokens = split(_load, '/');
    const std::string& inst = load_tokens[0];
    const std::string& output_pin = inst + ':' + load_tokens[2];
        
    //
    // write out load's parasitics
    //

    // TEMP(anton): Pi interconnect assumed
    std::string driver_node = _interconnect.get_driver_nodes()[0];
    std::string load_node   = _interconnect.get_load_nodes()[0];
    float cnear = _interconnect.get_cap(driver_node);
    float cfar  = _interconnect.get_cap(load_node);
    float res   = _interconnect.get_res(driver_node, load_node);
    
    fs << "* Load parasitics" << std::endl;
    fs << "Cnear_rise " << output_pin << "_rise 0 " << cnear << "f" << std::endl;
    fs << "Cnear_fall " << output_pin << "_fall 0 " << cnear << "f" << std::endl;

    // write out res and cfar only if they are non-zero
    // (most of time, load parasitics may be pure capacitance with only cnear)
    if ((res != 0.0) || (cfar != 0.0)) {
        fs << "Rres_rise " << output_pin << "_rise far_node_rise " << res << std::endl;
        fs << "Rres_fall " << output_pin << "_fall far_node_fall " << res << std::endl;
        fs << "Cfar_rise far_node_rise 0 " << cfar << "f" << std::endl;
        fs << "Cfar_fall far_node_fall 0 " << cfar << "f" << std::endl;
    }

}

bool Circuit::is_positive_unate(CellLib& cellLib) {

    // find arc with corresponding pin/related pin
    std::vector<std::string> driver_tokens = split(_driver, '/');
    const std::string& input_pin = driver_tokens[1];
    const std::string& output_pin = driver_tokens[2];

    // find cell
    Cell* cell = cellLib.get_cell(_driver_celltype);

    // by this time, we should always be able to find the cell
    assert(cell != nullptr);

    // find output pin (returns first output pin found)
    // TODO add support for multiple output pins
    CellPin* output_cellpin = cell->get_output_pin();
    assert(output_cellpin->get_name() == output_pin);

    // find arc
    CellArc* arc = output_cellpin->find_arc(input_pin);


    assert(arc);

    // check for unateness
    const std::string& timing_sense = arc->get_timing_sense();

    if (timing_sense == "positive_unate") {
        return true;
    }
    if (timing_sense == "negative_unate") {
        return false;
    }
    // we should never reach here
    // TODO need to add support for non-unate or empty strings
    assert(0);

}

void Circuit::write_spice_commands(std::fstream& fs, CellLib & cellLib) {


    fs << ".tran 0.1p " << get_sim_time() << "p 0 0.1p" << std::endl;

    std::vector<std::string> driver_tokens = split(_driver, '/');
    const std::string& inst = driver_tokens[0];
    const std::string& input_pin = inst + ':' + driver_tokens[1];
    const std::string& output_pin = inst + ':' + driver_tokens[2];

    const float delay_threshold = get_power_rail_voltage()/2.0;


    const float rise_slew_lower_threshold = cellLib.get_slew_lower_threshold_pct_rise() *
        get_power_rail_voltage() / 100.0;
    const float rise_slew_upper_threshold = cellLib.get_slew_upper_threshold_pct_rise() *
        get_power_rail_voltage() / 100.0;
    const float fall_slew_lower_threshold = cellLib.get_slew_lower_threshold_pct_fall() *
        get_power_rail_voltage() / 100.0;
    const float fall_slew_upper_threshold = cellLib.get_slew_upper_threshold_pct_fall() *
        get_power_rail_voltage() / 100.0;

    // spice measure format
    if (_spice_measure == SpiceMeasure::ngspice ) {

        if (is_positive_unate(cellLib)) {
            fs << ".measure tran rise_delay trig v(" << input_pin << "_rise) val="
               << delay_threshold << "v rise=1 targ v(" << output_pin << "_rise) val="
               << delay_threshold << " rise=1" << std::endl;
            fs << ".measure tran fall_delay trig v(" << input_pin << "_fall) val="
               << delay_threshold << "v fall=1 targ v(" << output_pin << "_fall) val="
               << delay_threshold << "v fall=1" << std::endl;
        } else {
            fs << ".measure tran fall_delay trig v(" << input_pin << "_fall) val="
               << delay_threshold << "v rise=1 targ v(" << output_pin << "_fall) val="
               << delay_threshold << "v fall=1" << std::endl;
            fs << ".measure tran rise_delay trig v(" << input_pin << "_rise) val="
               << delay_threshold << "v fall=1 targ v(" << output_pin << "_rise) val="
               << delay_threshold << "v rise=1" << std::endl;
        }

        fs << ".measure tran rise_slew trig v(" << output_pin << "_rise) val="
           << rise_slew_lower_threshold << "v rise=1 targ v(" << output_pin << "_rise) val="
           << rise_slew_upper_threshold << "v rise=1" << std::endl;
        
        fs << ".measure tran fall_slew trig v(" << output_pin << "_fall) val="
           << fall_slew_upper_threshold << "v fall=1 targ v(" << output_pin << "_fall) val="
           << fall_slew_lower_threshold << "v fall=1" << std::endl;

        return;
    }
    
    if (_spice_measure == SpiceMeasure::xyce ) {

        if (is_positive_unate(cellLib)) {
            fs << ".measure tran rise_delay trig v(" << input_pin << "_rise)="
               << delay_threshold << "v rise=1 targ v(" << output_pin << "_rise)="
               << delay_threshold << " rise=1" << std::endl;
            fs << ".measure tran fall_delay trig v(" << input_pin << "_fall)="
               << delay_threshold << "v fall=1 targ v(" << output_pin << "_fall)="
               << delay_threshold << "v fall=1" << std::endl;

        } else {
            fs << ".measure tran fall_delay trig v(" << input_pin << "_fall)="
               << delay_threshold << "v rise=1 targ v(" << output_pin << "_fall)="
               << delay_threshold << "v fall=1" << std::endl;
            fs << ".measure tran rise_delay trig v(" << input_pin << "_rise)="
               << delay_threshold << "v fall=1 targ v(" << output_pin << "_rise)="
               << delay_threshold << "v rise=1" << std::endl;
        }

        // slews
        fs << ".measure tran rise_slew trig v(" << output_pin << "_rise)="
           << rise_slew_lower_threshold << "v rise=1 targ v(" << output_pin << "_rise)="
           << rise_slew_upper_threshold << " rise=1" << std::endl;
        fs << ".measure tran fall_slew trig v(" << output_pin << "_fall)="
           << fall_slew_upper_threshold << "v fall=1 targ v(" << output_pin << "_fall)="
           << fall_slew_lower_threshold << " fall=1" << std::endl;
        return;
    }


}

void Circuit::write_spice_deck(const std::string& dirname, CellLib* cellLib, spef::Spef* spef,
                               const char* spice_lib_name, const char* spice_models,
                               const char* simulator) {


    // Note:  Units in SPICE file will be in MKS

    if (simulator && !strcmp(simulator, "xyce")) {
        set_xyce_measure();
    }

    std::fstream fs;
    std::string filename = dirname + "/" + _name + ".sp";
    fs.open(filename.c_str(), std::fstream::out);

    // title
    fs << "* SPICE circuit for " << _name << std::endl;

    // temperature
    fs << ".options device temp=25" << std::endl;
    fs << ".options device tnom=25" << std::endl;
    
    // write out spice lib
    fs << ".include " << spice_lib_name << std::endl;

    // write out spice models
    fs << ".include " << spice_models << std::endl;

    // write out voltage source
    this->write_spice_voltages(fs, *cellLib);

    // write out driver
    this->write_spice_driver(fs, *cellLib);
    
    // write out net
    this->write_spice_net(fs, *spef);
        
    // write out receiver
    this->write_spice_load(fs, *cellLib);

    // write out load's parasitics
    this->write_spice_load_parasitics(fs, *cellLib);

    // write out simulation commands
    this->write_spice_commands(fs, *cellLib);

    // end statement
    fs << ".end" << std::endl;

    // TODO: include models
    fs.close();
}


// patterns for reading simulation results
static const std::regex XYCE_RISE_DELAY_RE("RISE_DELAY = (.*)");
static const std::regex XYCE_FALL_DELAY_RE("FALL_DELAY = (.*)");
static const std::regex XYCE_RISE_SLEW_RE("RISE_SLEW = (.*)");
static const std::regex XYCE_FALL_SLEW_RE("FALL_SLEW = (.*)");
static const std::regex NGSPICE_RISE_DELAY_RE("rise_delay\\s+=\\s+(.*)");
static const std::regex NGSPICE_FALL_DELAY_RE("fall_delay\\s+=\\s+(.*)");
static const std::regex NGSPICE_RISE_SLEW_RE("rise_slew\\s+=\\s+(.*)");
static const std::regex NGSPICE_FALL_SLEW_RE("fall_slew\\s+=\\s+(.*)");

void Circuit::read_spice_results(const char* simulator, const std::string& results_filename) {

    std::regex rise_delay_re;
    std::regex fall_delay_re;
    std::regex rise_slew_re;
    std::regex fall_slew_re;
    
    if (!strcmp(simulator, "xyce")) {
        rise_delay_re = XYCE_RISE_DELAY_RE;
        fall_delay_re = XYCE_FALL_DELAY_RE;
        rise_slew_re = XYCE_RISE_SLEW_RE;
        fall_slew_re = XYCE_FALL_SLEW_RE;
    }

    if (!strcmp(simulator, "ngspice")) {
        rise_delay_re = NGSPICE_RISE_DELAY_RE;
        fall_delay_re = NGSPICE_FALL_DELAY_RE;
        rise_slew_re = NGSPICE_RISE_SLEW_RE;
        fall_slew_re = NGSPICE_FALL_SLEW_RE;
    }

    // output is in a mt0 file format
    std::ifstream infile(results_filename);
    
    std::string line;
    char* end;
    while (std::getline(infile, line)) {

        std::smatch match;
        if (std::regex_search(line, match, rise_delay_re) && match.size() > 1) {
            _spice_rise_delay = strtod(match.str(1).c_str(), &end) * 1.0e+12 ;
        }
            
        if (std::regex_search(line, match, fall_delay_re) && match.size() > 1) {
            _spice_fall_delay = strtod(match.str(1).c_str(), &end) * 1.0e+12 ;
        }
            
        if (std::regex_search(line, match, rise_slew_re) && match.size() > 1) {
            _spice_rise_slew = strtod(match.str(1).c_str(), &end) * 1.0e+12 ;
        }
            
        if (std::regex_search(line, match, fall_slew_re) && match.size() > 1) {
            _spice_fall_slew = strtod(match.str(1).c_str(), &end) * 1.0e+12 ;
        }
    }

    infile.close();

}

}
