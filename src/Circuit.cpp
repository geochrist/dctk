// Circuit
//
// A circuit is defined as a input voltage source, driver instance,
// driver interconnect, load instance, and load interconnect
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

Circuit& Circuit::set_input_waveform(const std::string& s) {
    this->_input_waveform = s;
    return *this;
}

const std::string& Circuit::get_name() {
        return _name;
    }

const std::string& Circuit::get_input_waveform() {
    return _input_waveform;
}

// driver
Circuit& Circuit::set_driver(const std::string& s) {

    this->_driver = s;
    return *this;
}
Circuit& Circuit::set_driver_celltype(const std::string& s) {

    this->_driver_celltype = s;
    return *this;
}

const std::string& Circuit::get_driver() {

    return _driver;
}
const std::string& Circuit::get_driver_celltype() {

    return _driver_celltype;
}

// receiver
Circuit& Circuit::set_load(const std::string& s) {

    this->_load = s;
    return *this;
}

Circuit& Circuit::set_load_celltype(const std::string& s) {

    this->_load_celltype = s;
    return *this;
}

Circuit& Circuit::set_load_interconnect(const std::string& s) {

    std::istringstream iss(s);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    // there should be exactly 3 tokens
    this->_load_interconnect.set_cnear(stof(results[0]));
    this->_load_interconnect.set_res(stof(results[1]));
    this->_load_interconnect.set_cfar(stof(results[2]));
    return *this;
}

const std::string& Circuit::get_load() {

    return _load;
}

const std::string& Circuit::get_load_celltype() {

    return _load_celltype;
}

const PiModel& Circuit::get_load_interconnect() {

    return _load_interconnect;
}

Circuit& Circuit::set_unused_loads(const std::string& s) {

  _unused_loads = s;
  return *this;
}

  const std::string& Circuit::get_unused_loads() {
  return _unused_loads;
    
}


void Circuit::dump() {
    std::cout << "Name = " << this->_name << std::endl;
    std::cout << "Input Waveform = " << this->_input_waveform << std::endl;
    std::cout << "Driver = " << this->_driver << std::endl;
    std::cout << "Driver Celltype = " << this->_driver_celltype << std::endl;
    std::cout << "Load = " << this->_load << std::endl;
    std::cout << "Load Celltype = " << this->_load_celltype << std::endl;
    std::cout << "load_interconnect = " << this->_load_interconnect.get_cnear()
              << " " << this->_load_interconnect.get_res()
              << " " << this->_load_interconnect.get_cfar()
              << std::endl;
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

    std::stringstream ss2;
    ss2 << this->_load_interconnect.get_cnear()
        << " " << this->_load_interconnect.get_res()
        << " " << this->_load_interconnect.get_cfar();

    emitter << YAML::Key << "load_interconnect" << YAML::Value << ss2.str();
    emitter << YAML::Key << "unused_loads" << YAML::Value << this->_unused_loads;

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
        float total_sim_time;
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

    // _load = I2/CK/Q
    std::vector<std::string> load_tokens = split(_load, '/');
    const std::string& inst = load_tokens[0];
    const std::string& output_pin = inst + ':' + load_tokens[2];
        
    //
    // write out load's parasitics
    //
    fs << "* Load parasitics" << std::endl;
    fs << "Cnear_rise " << output_pin << "_rise 0 " << _load_interconnect.get_cnear() << "f" << std::endl;
    fs << "Cnear_fall " << output_pin << "_fall 0 " << _load_interconnect.get_cnear() << "f" << std::endl;

    // write out res and cfar only if they are non-zero
    // (most of time, load parasitics may be pure capacitance with only cnear)
    if ((_load_interconnect.get_res() != 0.0) || (_load_interconnect.get_cfar() != 0.0)) {
        fs << "Rres_rise " << output_pin << "_rise far_node_rise " << _load_interconnect.get_res() << std::endl;
        fs << "Rres_fall " << output_pin << "_fall far_node_fall " << _load_interconnect.get_res() << std::endl;
        fs << "Cfar_rise far_node_rise 0 " << _load_interconnect.get_cfar() << "f" << std::endl;
        fs << "Cfar_fall far_node_fall 0 " << _load_interconnect.get_cfar() << "f" << std::endl;
    }

}

void Circuit::write_spice_unused_loads(std::fstream& fs, CellLib& cellLib) {


    // get list of unused loads
    std::vector<std::string> tokens = split(_unused_loads, ' ');
    // no unused loads
    if (tokens.size() == 0) {
      return;
    }

    // setup for voltage rail identification
    const std::map<std::string, float>& voltage_map = cellLib.get_voltage_map();
    const std::map<std::string, float>::const_iterator iterEnd = voltage_map.end();

    // tokens.size() should always be an even number
    int num_loads = tokens.size()/2;

    fs << "* Side Loads" << std::endl;
    
    for ( int i = 0; i < num_loads; i = i+2) {
      // figure out celltype and look up in library
      const std::string& celltype = tokens[i+1];
      Cell* unused_load_cell = cellLib.get_cell(celltype);

      // print out pins, in order
      std::vector<std::string> load_info = split(tokens[i], ':');
      const std::string& inst = load_info[0];

      std::vector<std::string> pins;
      std::vector<std::string> cellpins = unused_load_cell->get_pin_order();
      for (unsigned int j=0; j < cellpins.size(); j++) {
        // check to see if pin is a voltage rail
        if (voltage_map.find(cellpins[j]) != iterEnd) {
            pins.push_back(cellpins[j]);
        } else {
            pins.push_back(inst + ":" + cellpins[j]);
        }
      }

      // instance rise
      fs << "X" << inst << "_rise";

      // pins rise
      for (unsigned int k=0; k < pins.size(); k++) {
        fs << " " << pins[k] << "_rise";
      }
      // celltype
      fs << " " << celltype << std::endl;

      // instance fall
      fs << "X" << inst << "_fall";
      
      // pins rise
      for (unsigned int k=0; k< pins.size(); k++) {
        fs << " " << pins[k] << "_fall";
      }
      // celltype
      fs << " " << celltype << std::endl;
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

    // write out unused loads
    this->write_spice_unused_loads(fs, *cellLib);
    
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
