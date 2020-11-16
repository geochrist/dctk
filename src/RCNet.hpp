#ifndef DCTK_RCNET_HPP_
#define DCTK_RCNET_HPP_

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace dctk {

//    
// Representation of RC interconnect. The semantics are aligned with SPEF D_NET,
// i.e. its a collection of nodes, each node has a capacitance to ground, and
// pair-wire resistances with some other nodes.
//
// Currently we do not support cross-caps.
//
class RCNet {

public:

    // maintenance APIs
    RCNet();
    void set_name(std::string name);
    std::string get_name();
    void clear();

    // topology APIs - set... APIs return node name
    std::string add_driver_node(std::string name);
    std::string add_load_node(std::string name);
    std::string add_internal_node();
    
    std::vector<std::string> get_driver_nodes();
    std::vector<std::string> get_load_nodes();
    std::vector<std::string> get_internal_nodes();
    std::vector<std::string> get_connected_nodes(std::string node);
    
    // RC values APIs
    RCNet& set_cap(std::string node, float c);
    RCNet& set_res(std::string node1, std::string node2, float r);
    float get_cap(std::string node);
    float get_res(std::string node1, std::string node2);
    float get_total_cap();

    // dump/debug
    std::string to_string();
    
private:

    std::string _name      = "";
    float       _total_cap = NAN;
    
    // Data layout: node information is stored in the array of node_info's,
    // index in the array is the node_id (not visible externally; 0 = invalid)
    // the mapping from node name to node_id is stored in a separate map
    static const unsigned UNDEF_ID = 0;
    struct node_info {
        enum class type { UNDEF_TYPE = 0, DRIVER, LOAD, INTERNAL };
        std::string     _name = "UNDEF";
        unsigned        _id   = UNDEF_ID;
        type            _type = type::UNDEF_TYPE;
        float           _cap  = NAN;
        std::vector<std::pair<unsigned, float>> _res; // entry = <node_id, res>
        node_info() {}
        node_info(std::string name, type type, unsigned id) :
            _name(name), _id(id), _type(type) {}
        std::string to_string();
    };
    std::vector<node_info>                      _node_infos{
        node_info(/*default = UNDEF*/)};
    std::unordered_map<std::string, unsigned>   _node_id_map{
        { "UNDEF", 0/*UNDEF_ID*/ } };

    // helper APIs
    bool        _is_valid_node(std::string node);
    bool        _is_valid_id(unsigned id);
    unsigned    _node_to_id(std::string node);
    std::string _id_to_node(unsigned id);
    node_info&  _get_node_info(std::string node);
    unsigned    _add_node(std::string node, node_info::type type);
    unsigned    _get_max_node_id();
    std::vector<std::string> _get_nodes_by_type(node_info::type type);
};

}

#endif
