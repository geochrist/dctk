//    
// RCNet is the representation of RC interconnect. The semantics are aligned
// with SPEF D_NET, i.e. its a collection of nodes, each node has a capacitance
// to ground, and pair-wire resistances with some other nodes.
//
// Currently we do not support cross-caps.
//

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <src/RCNet.hpp>

static void run_unit_test(bool verb); // unit test

namespace dctk {

// maintenance APIs
RCNet::RCNet() {
    static bool run_test = false; // true;
    if (run_test) {
        run_test = false;
        run_unit_test(true/*verb*/);
    }
}
void RCNet::set_name(std::string name) {
    _name = name;
}
std::string RCNet::get_name() {
    return _name;
}
void RCNet::clear() {
    _name = "";
    _total_cap = NAN;
    _node_infos.clear();
    _node_infos.push_back(node_info(/*default = UNDEF*/));
    _node_id_map.clear();
    _node_id_map["UNDEF"] = UNDEF_ID;
}

// topology APIs - set... APIs return node name
std::string RCNet::add_driver_node(std::string name) {
    _add_node(name, node_info::type::DRIVER);
    return name;
}
std::string RCNet::add_load_node(std::string name) {
    _add_node(name, node_info::type::LOAD);
    return name;
}
std::string RCNet::add_internal_node() {
    // name for internal nodes is net/id
    std::string name = _name + ":" + std::to_string(_get_max_node_id() + 1);
    _add_node(name, node_info::type::INTERNAL);
    return name;
}
    
std::vector<std::string> RCNet::get_driver_nodes() {
    return _get_nodes_by_type(node_info::type::DRIVER);
}
std::vector<std::string> RCNet::get_load_nodes() {
    return _get_nodes_by_type(node_info::type::LOAD);
}
std::vector<std::string> RCNet::get_internal_nodes() {
    return _get_nodes_by_type(node_info::type::INTERNAL);  
}
std::vector<std::string> RCNet::get_connected_nodes(std::string node) {
    std::vector<std::string> nodes;
    node_info& ni = _node_infos[_node_to_id(node)];
    for (auto& e : ni._res) {
        nodes.push_back(_id_to_node(e.first));
    }
    return nodes;
}
    
// RC values APIs
RCNet& RCNet::set_cap(std::string node, float c) {
    _get_node_info(node)._cap = c;
    _total_cap = std::isnan(_total_cap) ? c : _total_cap + c;
    return *this;
}
RCNet& RCNet::set_res(std::string node1, std::string node2, float r) {
    node_info& ni1 = _get_node_info(node1);
    node_info& ni2 = _get_node_info(node2);
    ni1._res.push_back({ni2._id, r});
    ni2._res.push_back({ni1._id, r});
    return *this;
}

float RCNet::get_cap(std::string node) {
    return _get_node_info(node)._cap;
}
float RCNet::get_res(std::string node1, std::string node2) {
    float res = NAN;
    unsigned id2 = _node_to_id(node2);
    for (auto& ri : _get_node_info(node1)._res) {
        if (ri.first == id2) {
            res = ri.second;
        }
    }
    return res;
}
float RCNet::get_total_cap() {
    return _total_cap;
}

// dump/debug
std::string RCNet::to_string() {
    std::stringstream str;
    str << "name = " << _name << std::endl;
    for (unsigned i = 1; i < _node_infos.size(); ++i) {
        str << _node_infos[i].to_string() << std::endl;
    }
    return str.str();
}
std::string RCNet::node_info::to_string() {
    std::stringstream str;
    char type = '?';
    switch (_type) {
    case node_info::type::UNDEF_TYPE: type = '?'; break;
    case node_info::type::DRIVER: type = 'D'; break;
    case node_info::type::LOAD: type = 'L'; break;
    case node_info::type::INTERNAL: type = 'I'; break;
    }
    str << _name << "(" << type << ")"
        << " id=" << _id
        << " cap=" << _cap
        << " res={ "
        << std::flush;
    for (auto& ri : _res) {
        str << "{" << ri.first << ", " << ri.second << "} ";
    }
    str << "}";
    return str.str();
}

// helper APIs
bool RCNet::_is_valid_node(std::string node) {
    return _node_id_map.count(node);
}
bool RCNet::_is_valid_id(unsigned id) {
    return id > UNDEF_ID && id < _node_infos.size();
}
unsigned RCNet::_node_to_id(std::string node) {
    assert(_is_valid_node(node) && "invalid node");
    return _node_id_map.at(node);
}
std::string RCNet::_id_to_node(unsigned id) {
    assert(_is_valid_id(id) && "invalid node id");
    return _node_infos.at(id)._name;
}
RCNet::node_info& RCNet::_get_node_info(std::string node) {
    assert(_is_valid_node(node) && "invalid node");
    return _node_infos[_node_to_id(node)];
}
unsigned RCNet::_add_node(std::string node, node_info::type type) {
    assert(!_is_valid_node(node) && "attempted to add duplicate node");
    unsigned id = _node_infos.size();
    _node_infos.push_back(node_info(node, type, id));
    _node_id_map[node] = id;
    return id;
}
unsigned RCNet::_get_max_node_id() {
    return _node_infos.back()._id;
}
std::vector<std::string> RCNet::_get_nodes_by_type(node_info::type type) {
    // NOTE(anton): obviously inefficient; optimize if needed
    std::vector<std::string> nodes;
    for (node_info& ni : _node_infos) {
        if (ni._type == type) {
            nodes.push_back(ni._name);
        }
    }
    return nodes;
}

}

// unit-test
static void run_unit_test(bool verb) {
    // make a "wishbone" net
    dctk::RCNet net;
    net.set_name("test");
    std::string dr1 = net.add_driver_node("BUF1/Z");
    std::string dr2 = net.add_driver_node("BUF2/Z");
    std::string ld1 = net.add_load_node("INV1/A");
    std::string ld2 = net.add_load_node("INV2/A");
    std::string n1 = net.add_internal_node();
    std::string n2 = net.add_internal_node();
    // caps
    net.set_cap(dr1, 1.0);
    net.set_cap(dr2, 2.0);
    net.set_cap(ld1, 3.0);
    net.set_cap(ld2, 4.0);
    net.set_cap(n1, 5.0);
    net.set_cap(n2, 6.0);
    // res
    net.set_res(dr1, n1, 7.0);
    net.set_res(dr2, n1, 8.0);
    net.set_res(n1, n2, 9.0);
    net.set_res(n2, ld1, 10.0);
    net.set_res(n2, ld2, 11.0);
    // done
    if (verb) {
        std::cout << "[TEST] constructed interconnect:" << std::endl;
        std::cout << net.to_string() << std::endl;
    }
    // test
    assert(net.get_cap(dr1) == 1.0);
    assert(net.get_cap(dr2) == 2.0);
    assert(net.get_cap(ld1) == 3.0);
    assert(net.get_cap(ld2) == 4.0);
    assert(net.get_cap(n1) == 5.0);    
    assert(net.get_cap(n2) == 6.0);
    assert(net.get_total_cap() == 21.0);
    assert(net.get_res(dr1, n1) == 7.0);
    assert(net.get_res(dr1, n1) == net.get_res(n1, dr1));
    assert(net.get_res(dr2, n1) == 8.0);
    assert(net.get_res(dr2, n1) == net.get_res(n1, dr2));
    assert(net.get_res(n1, n2) == 9.0);
    assert(net.get_res(n1, n2) == net.get_res(n2, n1));
    assert(net.get_res(n2, ld1) == 10.0);
    assert(net.get_res(n2, ld1) == net.get_res(ld1, n2));
    assert(net.get_res(n2, ld2) == 11.0);
    assert(net.get_res(n2, ld2) == net.get_res(ld2, n2));
    std::vector<std::string> cn1 = net.get_connected_nodes(dr1);
    assert(cn1.size() == 1 && cn1[0] == n1);
    std::vector<std::string> cn2 = net.get_connected_nodes(dr2);
    assert(cn2.size() == 1 && cn2[0] == n1);
    std::vector<std::string> cn3 = net.get_connected_nodes(n1);
    assert(cn3.size() == 3 &&
           (std::count(cn3.begin(), cn3.end(), dr1) == 1) &&
           (std::count(cn3.begin(), cn3.end(), dr2) == 1) &&
           (std::count(cn3.begin(), cn3.end(), n2) == 1));
    std::vector<std::string> cn4 = net.get_connected_nodes(n2);
    assert(cn4.size() == 3 &&
           (std::count(cn4.begin(), cn4.end(), ld1) == 1) &&
           (std::count(cn4.begin(), cn4.end(), ld2) == 1) &&
           (std::count(cn4.begin(), cn4.end(), n1) == 1));
    std::vector<std::string> cn5 = net.get_connected_nodes(ld1);
    assert(cn5.size() == 1 && cn5[0] == n2);
    std::vector<std::string> cn6 = net.get_connected_nodes(ld2);
    assert(cn6.size() == 1 && cn6[0] == n2);
    if (verb) {
        std::cout << "[TEST] passed" << std::endl;
    }
}
    
