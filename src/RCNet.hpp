#ifndef DCTK_RCNET_HPP_
#define DCTK_RCNET_HPP_

#include <cmath>
#include <string>
#include <sstream>
#include <vector>

namespace dctk {

//    
// Representation of RC interconnect. The semantics are aligned with SPEF D_NET,
// i.e. its a collection of nodes, each node has a capacitance to ground, and
// pair-wire resistances with some other nodes.
//
// TEMP(anton): only supports Pi networks
//
class RCNet {

public:

    RCNet() {
    }

    // maintenance APIs
    void set_name(std::string name) {
        _name = name;
    }
    std::string get_name() {
        return _name;
    }
    void clear() {
        _name = _driver = _load = "";
        _cnear = _cfar = _res = NAN;
    }

    // topology APIs - set... APIs return node name
    const std::string& add_driver(std::string name) {
        _driver = name;
        return _driver;
    }
    const std::string& add_load(std::string name) {
        _load = name;
        return _load;
    }
    std::string add_internal() {
        return "";
    }
    
    std::vector<std::string> get_drivers() {
        return {_driver};
    }
    std::vector<std::string> get_loads() {
        return {_load};
    }
    std::vector<std::string> get_internals() {
        return {};
    }
    std::vector<std::string> get_connected_nodes(std::string node) {
        if (node == _driver) {
            return get_loads();
        } else if (node == _load) {
            return get_drivers();
        } else {
            return {};
        }
    }
    
    // RC values APIs
    RCNet& set_cap(std::string node, float c) {
        if (node == _driver) {
            _cnear = c;
        } else if (node == _load) {
            _cfar = c;
        }
        return *this;
    }
    RCNet& set_res(std::string node1, std::string node2, float r) {
        if ((node1 == _driver && node2 == _load) ||
            (node2 == _driver && node1 == _load)) {
            _res = r;
        }
        return *this;
    }

    float get_cap(std::string node) {
        return (node == _driver) ? _cnear : ((node == _load) ? _cfar : NAN);
    }
    float get_res(std::string node1, std::string node2) {
        return ((node1 == _driver && node2 == _load) ||
                (node2 == _driver && node1 == _load)) ? _res : NAN;
    }
    float get_total_cap() {
        return _cnear + _cfar;
    }

    // dump/debug
    std::string to_string() {
        std::stringstream str;
        str << _cnear << " " << _res << " " << " " << _cfar;
        return str.str();
    }
    
private:

    // TEMP: equivalent of PiModel
    std::string _name   = "";
    std::string _driver = "";
    std::string _load   = "";
    float _cnear        = NAN;
    float _cfar         = NAN;
    float _res          = NAN;

};

}

#endif
