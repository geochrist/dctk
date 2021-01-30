#include "RandomRCNet.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <getopt.h>
#include <string.h>
#include <limits>

// dctk definitions
#include "dctk.hpp"
#include "Circuit.hpp"

// Liberty reader
#include "liberty_reader.hpp"

using namespace std;

LayerRCData::LayerRCData( std::string & tech_node )
{
    res_scale_ = 1.0;
    cap_scale_ = 1.0e-15;
    len_scale_ = 1.0e-6;

    if( tech_node == "7nm" ) {
        create_7nm_data();
        return;
    } else if( tech_node == "14nm" ) {
        create_14nm_data();
        return;
    } else if( tech_node == "20nm" ) {
        create_20nm_data();
        return;
    } else if( tech_node == "28nm" ) {
        create_28nm_data();
        return;
    } else if( tech_node == "40nm" ) {
        create_40nm_data();
        return;
    } else {
        std::cout << "Error: unknown technology node " << tech_node << std::endl;
    }
}



void LayerRCData::create_7nm_data()
{
    double res[16] = {100.0, 100.0, 20.0, 20.0, 10.0, 10.0, 10.0, 2.5, 2.5, 2.5, 2.5, 1.0, 1.0, 0.1, 0.1, 0.01};
    double cap[16] = {0.15, 0.15, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5};
    double via[16] = {50.0, 50.0, 10.0, 10.0, 5.0, 5.0, 5.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 0.5, 0.5, 0.05};

    all_layer_data_.clear();
    for (int i=0; i<16; i++) {
        VRCData new_layer( i, via[i], res[i], cap[i] );
        all_layer_data_.push_back( new_layer );
    }
}

void LayerRCData::create_14nm_data()
{
    double res[14] = {20.0, 20.0, 10.0, 10.0, 10.0, 2.5, 2.5, 2.5, 2.5, 1.0, 1.0, 0.1, 0.1, 0.01};
    double cap[14] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5};
    double via[14] = {10.0, 10.0, 5.0, 5.0, 5.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 0.5, 0.5, 0.05};

    all_layer_data_.clear();
    for (int i=0; i<14; i++) {
        VRCData new_layer( i, via[i], res[i], cap[i] );
        all_layer_data_.push_back( new_layer );
    }
}

void LayerRCData::create_20nm_data()
{
    double res[13] = {10.0, 10.0, 2.5, 2.5, 2.5, 1.0, 1.0, 1.0, 0.1, 0.1, 0.025, 0.025, 0.01};
    double cap[13] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5};
    double via[13] = {5.0, 5.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.1, 0.1, 0.05};

    all_layer_data_.clear();
    for (int i=0; i<13; i++) {
        VRCData new_layer( i, via[i], res[i], cap[i] );
        all_layer_data_.push_back( new_layer );
    }
}

void LayerRCData::create_28nm_data()
{
    double res[12] = {2.5, 2.5, 1.0, 1.0, 1.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.01, 0.001};
    double cap[12] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    double via[12] = {2.0, 2.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.1, 0.1, 0.05, 0.05};

    all_layer_data_.clear();
    for (int i=0; i<12; i++) {
        VRCData new_layer( i, via[i], res[i], cap[i] );
        all_layer_data_.push_back( new_layer );
    }
}

void LayerRCData::create_40nm_data()
{
    double res[11] = {1.0, 1.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.01, 0.01, 0.001};
    double cap[11] = {0.2, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    double via[11] = {1.0, 1.0, 0.5, 0.5, 0.5, 0.1, 0.1, 0.1, 0.05, 0.05, 0.05};

    all_layer_data_.clear();
    for (int i=0; i<11; i++) {
        VRCData new_layer( i, via[i], res[i], cap[i] );
        all_layer_data_.push_back( new_layer );
    }
}

int LayerRCData::get_num_layers() const
{
    return all_layer_data_.size();
}

int LayerRCData::get_rand_max_layer_index() const
{
    int num_lyrs = all_layer_data_.size();
    if( num_lyrs > 0 ) {
        int lyr_indx = rand() % num_lyrs + 1;
        return lyr_indx;
    } else {
        return 0;
    }
}

int LayerRCData::get_guided_layer_lengths( double max_total_len )
{
    int num_lyrs = all_layer_data_.size();

    // group the layers based on their RC constant value
    std::vector<double> lyr_rc;
    std::vector<int> lyr_grp;
    double last_rc = 1.0e+10;
    int last_indx = 0;
    double avg_ratio = 0.0;
    double new_ratio = 1.0;
    for(int i=0; i<num_lyrs; i++){
        double new_rc = all_layer_data_[i].tot_cap_ * all_layer_data_[i].lyr_res_;
        if(new_rc < last_rc){
            if( last_indx > 1 ){
                new_ratio = last_rc / new_rc;
            }
            lyr_rc.push_back( new_rc );
            last_indx = last_indx + 1;
            last_rc = new_rc;
            avg_ratio = avg_ratio + new_ratio;
        }
        lyr_grp.push_back( last_indx );
        std::cout << "for layer " << i << " rc = " << last_rc << " grp indx = " << last_indx << std::endl;
    }
    std::cout << "Identified " << last_indx << " metal rc distinct groups" << std::endl;
    avg_ratio = avg_ratio / num_lyrs;
    std::cout << "Identified avg rc improvement between metal layers to be " << avg_ratio << std::endl;

    double total_ratio = 1.0;
    for(int i=1; i<last_indx; i++){
        total_ratio = total_ratio + std::pow( avg_ratio, i);
    }

    double base_len = max_total_len / std::pow(avg_ratio, last_indx);
    std::cout << "Identified first group len to be " << base_len << std::endl;
    for(int i= 0; i<num_lyrs; i++){
        double last_len = 0.0;
        double ith_len = base_len * std::pow( avg_ratio, lyr_grp[i] );
        if( lyr_grp[i] > 1){
            last_len = base_len * std::pow( avg_ratio, lyr_grp[i]-1 );
        } else {
            last_len = 0.0;
        }
        all_layer_data_[i].min_len_ = last_len;
        all_layer_data_[i].max_len_ = ith_len;
        
        std::cout << "For layer " << i << " identified driving length range to be from " << last_len << " to " << ith_len << std::endl;
    }
    return 1;
}


int LayerRCData::get_guided_layer_index_by_len ( double net_len ) const
{
    int num_lyrs = all_layer_data_.size();
    std::vector<int> proper_lyr_indices;
    for( int i=0; i<num_lyrs; i++ ){
        if( all_layer_data_[i].min_len_ <= net_len && net_len <= all_layer_data_[i].max_len_ ){
            proper_lyr_indices.push_back( i );
        }
    }
    if( proper_lyr_indices.size() == 0 ){
        // maybe the length is larger than the max?
        proper_lyr_indices.push_back( num_lyrs-1 ); 
    }
    int lyr_pos = rand() % proper_lyr_indices.size();
    int lyr_index = proper_lyr_indices[lyr_pos];

    return lyr_index;
}

int LayerRCData::get_guided_layer_index_by_cap ( double net_cap ) const
{
    int num_lyrs = all_layer_data_.size();
    std::vector<int> proper_lyr_indices;
    for( int i=0; i<num_lyrs; i++ ){
        double net_len = net_cap / all_layer_data_[i].tot_cap_;
        if( all_layer_data_[i].min_len_ <= net_len && net_len <= all_layer_data_[i].max_len_ ){
            proper_lyr_indices.push_back( i );
        }
    }
    if( proper_lyr_indices.size() == 0 ){
        // maybe the length is larger than the max?
        proper_lyr_indices.push_back( num_lyrs-1 );
    }
    int lyr_pos = rand() % proper_lyr_indices.size();
    int lyr_index = proper_lyr_indices[lyr_pos];

    return lyr_index;
}


double LayerRCData::get_via_stack_res( int first_layer_index, int second_layer_index ) const
{
    int start_lyr = first_layer_index;
    int end_lyr = second_layer_index;
    if( start_lyr > end_lyr ) {
        start_lyr = second_layer_index;
        end_lyr = first_layer_index;
    }
    double via_stack = 0.0;
    for(int i=start_lyr+1; i<=end_lyr; i++) {
        via_stack = via_stack + all_layer_data_[i].via_res_;
    }
    return via_stack;
}

double LayerRCData::get_shape_layer_res( int layer_index, double layer_length ) const
{
    if( layer_index < 0 || layer_index >= (int) all_layer_data_.size() ) {
        return 0.0;
    } else {
        double shape_res = all_layer_data_[layer_index].lyr_res_ * layer_length;
        return shape_res;
    }
}

double LayerRCData::get_shape_layer_cap( int layer_index, double layer_length ) const
{
    if( layer_index < 0 || layer_index >= (int) all_layer_data_.size() ) {
        return 0.0;
    } else {
        double shape_cap = all_layer_data_[layer_index].tot_cap_ * layer_length;
        return shape_cap;
    }
}

double LayerRCData::get_layer_res_per_len( int layer_index ) const
{
    if( layer_index < 0 || layer_index >= (int) all_layer_data_.size() ) {
        return 0.0;
    } else {
        double shape_res = all_layer_data_[layer_index].lyr_res_;
        return shape_res;
    }
}

double LayerRCData::get_layer_cap_per_len( int layer_index ) const
{
    if( layer_index < 0 || layer_index >= (int) all_layer_data_.size() ) {
        return 0.0;
    } else {
        double shape_cap = all_layer_data_[layer_index].tot_cap_;
        return shape_cap;
    }
}


int LayerRCData::get_wire_segment_vrcdata( int parent_layer_index, int segment_layer_index,
        const std::vector<double> & total_segment_lengths,
        double & via_stack_res,
        std::vector<double> & seg_res,
        std::vector<double> & seg_cap ) const
{
    via_stack_res = get_via_stack_res( parent_layer_index, segment_layer_index );
    seg_res.clear();
    seg_cap.clear();
    for(std::vector<double>::const_iterator sl_it = total_segment_lengths.begin();
            sl_it != total_segment_lengths.end(); ++sl_it ) {
        double seg_length = *sl_it;
        double shape_res = get_shape_layer_res(segment_layer_index, seg_length);
        double shape_cap = get_shape_layer_cap(segment_layer_index, seg_length);
        seg_res.push_back( shape_res );
        seg_cap.push_back( shape_cap );
    }
    if( via_stack_res <= 0.0 ) {
        return 0;
    }
    else {
        return 1;
    }
}


bool RandomRCNet::populate_pimodel_data( const std::string & net_name,
        const std::string & drv_node, const std::string & rcvr_node,
        double total_length, int max_layer_index, const LayerRCData & layer_data )
{
    if( max_layer_index < 0 or max_layer_index >= layer_data.get_num_layers() ) {
        return false;
    }

    net_index_ = net_name;
    pi_model_ = true;
    all_rc_devices_.clear();
    all_node_types_.clear();

    double via_val = layer_data.get_via_stack_res( -1, max_layer_index );
    double shape_res = layer_data.get_shape_layer_res(max_layer_index, total_length);
    shape_res = shape_res + 2.0 * via_val;
    double shape_cap = layer_data.get_shape_layer_cap(max_layer_index, total_length);

    std::string gnd_node = "gnd";
    RCDevice near_cap( 3, 1, drv_node, gnd_node, shape_cap*0.5 );
    all_rc_devices_.push_back( near_cap );

    RCDevice shp_res( 2, 2, drv_node, rcvr_node, shape_res);
    all_rc_devices_.push_back( shp_res );
    RCDevice far_cap( 3, 2, rcvr_node, gnd_node, shape_cap*0.5 );
    all_rc_devices_.push_back( far_cap );

    all_node_types_.insert( std::map<std::string, int>::value_type( gnd_node, 0 ) );  // gnd
    all_node_types_.insert( std::map<std::string, int>::value_type( drv_node, 1 ) );  // drv
    all_node_types_.insert( std::map<std::string, int>::value_type( rcvr_node, 2 ) );  // rcvr

    return true;
}

bool RandomRCNet::populate_pimodel_data_scaled( const std::string & net_name,
        const std::string & drv_node, const std::string & rcvr_node,
        double total_length, int max_layer_index, const LayerRCData & layer_data,
        double total_cap, double cnear_cfar_ratio, double res_scale )
{
    if( max_layer_index < 0 or max_layer_index >= layer_data.get_num_layers() ) {
        return false;
    }

    net_index_ = net_name;
    pi_model_ = true;
    all_rc_devices_.clear();
    all_node_types_.clear();

    double via_val = layer_data.get_via_stack_res( -1, max_layer_index );
    double shape_res = layer_data.get_shape_layer_res(max_layer_index, total_length);
    shape_res = shape_res + 2.0 * via_val;
    double shape_cap = layer_data.get_shape_layer_cap(max_layer_index, total_length);

    // Now scale resistances and capacitances linearly to match given cap
    double factor = total_cap / shape_cap; 
    shape_res = factor * shape_res;
    shape_cap = factor * shape_cap;

    // Further adjust base on ratios
    double res  = shape_res * res_scale;
    double cfar = shape_cap / (1.0 +  cnear_cfar_ratio);
    double cnear = cnear_cfar_ratio * cfar;

    std::string gnd_node = "gnd";
    RCDevice near_cap( 3, 1, drv_node, gnd_node, cnear );
    all_rc_devices_.push_back( near_cap );

    RCDevice shp_res( 2, 2, drv_node, rcvr_node, res);
    all_rc_devices_.push_back( shp_res );
    RCDevice far_cap( 3, 2, rcvr_node, gnd_node, cfar );
    all_rc_devices_.push_back( far_cap );

    all_node_types_.insert( std::map<std::string, int>::value_type( gnd_node, 0 ) );  // gnd
    all_node_types_.insert( std::map<std::string, int>::value_type( drv_node, 1 ) );  // drv
    all_node_types_.insert( std::map<std::string, int>::value_type( rcvr_node, 2 ) );  // rcvr

    return true;
}


bool RandomRCNet::populate_new_node( std::string & node_name, int node_type, int node_layer )
{
    std::map< std::string, int >::const_iterator fn_it = all_node_types_.find( node_name );
    if( fn_it == all_node_types_.end() ){
        all_node_types_.insert( std::map<std::string, int>::value_type( node_name, node_type ) );
        all_node_layers_.insert( std::map<std::string, int>::value_type( node_name, node_layer ) );
    }

    return true;
}

bool RandomRCNet::populate_rc_piece( std::string & from_node, std::string & to_node,
                            double res, double cap, int layer_indx, int res_indx, int cap_indx )
{
    populate_new_node( from_node, 3, layer_indx );
    populate_new_node( to_node, 3, layer_indx );

    RCDevice shp_res( 2, res_indx, from_node, to_node, res);
    all_rc_devices_.push_back( shp_res );

    std::string gnd_node = "gnd";
    RCDevice shp_cap( 3, cap_indx, to_node, gnd_node, cap);
    all_rc_devices_.push_back( shp_cap );

    return true;
}

bool RandomRCNet::populate_rc_segment( std::string & attch_node, std::string & rcvr_node, int layer_indx,
                            double attch_via_val, double rcvr_via_val, double seg_res, double seg_cap,
                            int & node_count, int & res_indx, int & cap_indx, int seg_num_pieces )
{
    if( seg_num_pieces < 1){
        return false;
    }

    // attachment node must already exist
    // create the first node of the new segment
    std::string first_node = net_index_ + ":" + std::to_string(node_count);  
    node_count++;
 
    // distribute the shape res and cap in equal parts
    double res = seg_res / seg_num_pieces;
    double cap = seg_cap / (seg_num_pieces + 1);

    // create  a via res and a cap to gnd: same as any other rc_piece
    populate_rc_piece( attch_node, first_node, attch_via_val, cap, layer_indx, res_indx, cap_indx );
    res_indx++;
    cap_indx++;
    
    // create the rest of the rc pieces for this segment
    for( int i=0; i<seg_num_pieces; i++){
        std::string from_node = net_index_ + ":" + std::to_string(node_count-1);  
        std::string to_node = net_index_ + ":" + std::to_string(node_count);  
        node_count++;
        populate_rc_piece( from_node, to_node, res, cap, layer_indx, res_indx, cap_indx );
        res_indx++;
        cap_indx++;
    }

    // attach the via down to the receiver node
    populate_new_node( rcvr_node, 2, -1 );
    std::string last_node = net_index_ + ":" + std::to_string(node_count-1);
    RCDevice rcv_via( 1, res_indx, last_node, rcvr_node, rcvr_via_val);
    res_indx++;
    all_rc_devices_.push_back( rcv_via );

    return true;
}

bool RandomRCNet::create_random_net_segment_lengths( double total_length, int num_receivers,
                                     std::vector<double> & net_lengths )
{
    if(num_receivers == 1){
        net_lengths.clear();
        net_lengths.push_back( total_length );
        return true;
    }

    std::vector<double> rcv_pctgs;
    double sum_lengths = 0.0;
    for(int i=0; i<num_receivers; i++){
        double rcvr_len = 1.0 + double(rand() % 100);
        rcv_pctgs.push_back( rcvr_len );
        sum_lengths += rcvr_len;
    }

    net_lengths.clear();
    for(int i=0; i<num_receivers; i++){
        double rcvr_len = rcv_pctgs[i] / sum_lengths * total_length;
        net_lengths.push_back( rcvr_len );
        std::cout << "Segment " << i << " length " << rcvr_len << std::endl;
    }

    return true;
}

bool RandomRCNet::create_random_net_segment_caps( double total_cap, int num_receivers,
                                     int max_layer_num, const LayerRCData & layer_data,
                                     std::vector<int> & net_layers,
                                     std::vector<double> & net_lengths )
{
    if(num_receivers == 1){
        net_lengths.clear();
        net_layers.clear();
        double cap_scale = layer_data.get_layer_cap_per_len(max_layer_num);
        if( cap_scale == 0.0 ){
            return false;
        } else {
            double cap_len = total_cap / cap_scale;
            net_lengths.push_back( cap_len );
            net_layers.push_back( max_layer_num );
            return true;
        }
    } else {
        net_lengths.clear();
        net_layers.clear();

        // create a set of random percentages of the total len/cap that is allocated to each receiver
        std::vector<double> rcv_pctgs;
        double sum_lengths = 0.0;
        for(int i=0; i<num_receivers; i++){
            double rcvr_len = 1.0 + double(rand() % 100);
            rcv_pctgs.push_back( rcvr_len );
            sum_lengths += rcvr_len;
        }
        for(int i=0; i<num_receivers; i++){
            rcv_pctgs[i] = rcv_pctgs[i] / sum_lengths;
        }

        // the first segment is on the max layer
        double cap_scale = layer_data.get_layer_cap_per_len(max_layer_num);
        double cap_len = 0.0;
        if(cap_scale == 0.0){
            return false;
        } else {
            cap_len = total_cap * rcv_pctgs[0] / cap_scale;
            net_lengths.push_back( cap_len );
            net_layers.push_back( max_layer_num );
        }

        // create the rest of the segments on random layers, but not above the max layer
        for(int i=1; i<num_receivers; i++){
            int rcvr_layer = rand() % max_layer_num;
            cap_scale = layer_data.get_layer_cap_per_len(rcvr_layer);
            cap_len = total_cap * rcv_pctgs[i] / cap_scale;
            net_lengths.push_back( cap_len );
            net_layers.push_back( rcvr_layer );
            std::cout << "Segment " << i << " length " << cap_len << " percentage " << rcv_pctgs[i] << std::endl;
        }
    }
    return true;
}


bool RandomRCNet::populate_net_data( const std::string & net_name,
                                     const std::string & drv_node, const std::vector<std::string > & receivers,
                                     double total_cap, int max_layer_index, const LayerRCData & layer_data )
{
    if( receivers.size() < 1 or max_layer_index < 0 or max_layer_index >= layer_data.get_num_layers() ) {
        return false;
    }

    if( receivers.size() > 1 && max_layer_index < 1 ){
        std::cout << "Warning: unable to route net with multiple receivers on first layer only. Changing max layer index to 1" << std::endl;
        max_layer_index = 1;
    }

    net_index_ = net_name;

    int num_sections_per_segment = 4;  // divide every straight segment into 4 pieces
    int num_receivers = receivers.size();

    // create a set of random layer indices and random lengths such that total cap is matched
    std::vector<double> net_segment_lengths;
    std::vector<int> net_segment_layers;
    create_random_net_segment_caps( total_cap, num_receivers, max_layer_index, layer_data, net_segment_layers, net_segment_lengths);

    // initialize the node and device counters
    int node_count = 0;
    int res_count = 1;
    int cap_count = 1;

    // create the driver node first
    std::string drv_node_name = drv_node;
    populate_new_node( drv_node_name, 1, -1);

    // the first segment, regardless of its length, will be "routed" on the highest layer
    double drv_via_val = layer_data.get_via_stack_res( -1, max_layer_index );
    double shape_res = layer_data.get_shape_layer_res(max_layer_index, net_segment_lengths[0]);
    double shape_cap = layer_data.get_shape_layer_cap(max_layer_index, net_segment_lengths[0]);
    std::string rcv_node_name = receivers[0];
    populate_rc_segment( drv_node_name, rcv_node_name, max_layer_index,
                            drv_via_val, drv_via_val, shape_res, shape_cap, node_count,
                            res_count, cap_count, num_sections_per_segment );

    for(int seg_indx = 1; seg_indx < num_receivers; seg_indx++ ){
        // choose an attachment node at random among all the nodes already created
        int seg_node_index = rand() % node_count;
        std::string att_node_name = net_index_ + ":" + std::to_string(seg_node_index);
        std::string rcv_node_name = receivers[seg_indx];
        int attach_node_layer = all_node_layers_[att_node_name];

        // the segment layer
        int new_seg_layer = net_segment_layers[seg_indx];

        std::cout << "Chosen attach node " << att_node_name << " on layer " << attach_node_layer << " new segment on layer " << new_seg_layer << std::endl;

        double att_via_val = 0.01;
        if( new_seg_layer != attach_node_layer){
            att_via_val = layer_data.get_via_stack_res( attach_node_layer, new_seg_layer );
        }
        double rcv_via_val = layer_data.get_via_stack_res( -1, new_seg_layer );
        double shape_res = layer_data.get_shape_layer_res(new_seg_layer, net_segment_lengths[seg_indx]);
        double shape_cap = layer_data.get_shape_layer_cap(new_seg_layer, net_segment_lengths[seg_indx]);
        populate_rc_segment( att_node_name, rcv_node_name, new_seg_layer,
                            att_via_val, rcv_via_val, shape_res, shape_cap, node_count,
                            res_count, cap_count, num_sections_per_segment );
    }
    
    
    return true;

}


void RandomRCNet::dump_spice_subckt_net( std::ofstream & ofs,
        double res_scale, double cap_scale ) const
{
    ofs << std::endl;
    ofs << ".SUBCKT " << net_index_ << " " << drv_node_;
    for( std::vector<std::string>::const_iterator rcv_it = rcv_nodes_.begin();
            rcv_it != rcv_nodes_.end(); ++rcv_it) {
        const std::string & rcv_node = *rcv_it;
        ofs << " " << rcv_node;
    }
    ofs << std::endl;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
            dev_it !=  all_rc_devices_.end(); ++dev_it ) {
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 1 ) {
            ofs << "RV" << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;
        } else if( dev.dev_typ_ == 2 ) {
            ofs << "RW" << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;
        } else if( dev.dev_typ_ == 3 ) {
            ofs << "CG" << dev.dev_index_ << " " << dev.dev_n1_ << " 0 " << dev.dev_val_*cap_scale << std::endl;
        }
    }

    ofs << ".ENDS " << net_index_ << std::endl;

    return;
}


float RandomRCNet::get_total_cap(float cap_scale) const
{
    float tot_cap = 0.0;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
            dev_it !=  all_rc_devices_.end(); ++dev_it ) {
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 3 ) {
            tot_cap += dev.dev_val_ * cap_scale;
        }
    }
    return tot_cap;
}

void RandomRCNet::dump_spef_dnet( std::ofstream & ofs, double res_scale, double cap_scale ) const
{
    double tot_cap = 0.0;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
            dev_it !=  all_rc_devices_.end(); ++dev_it ) {
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 3 ) {
            tot_cap += dev.dev_val_ * cap_scale;
        }
    }

    ofs << std::endl;
    ofs << "*D_NET " << net_index_ << " " << tot_cap << std::endl;
    ofs << "*CONN" << std::endl;
    for ( std::map< std::string, int >::const_iterator port_it = all_node_types_.begin();
            port_it != all_node_types_.end(); ++port_it) {
        if( port_it->second == 1 ) {
            ofs << "*I " << port_it->first << " O" << std::endl;
        } else if(  port_it->second == 2 ) {
            ofs << "*I " << port_it->first << " I" << std::endl;
        }
    }

    ofs << "*CAP" << std::endl;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
            dev_it !=  all_rc_devices_.end(); ++dev_it ) {
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 3 ) {
            ofs << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_val_*cap_scale << std::endl;
        }
    }

    ofs << "*RES" << std::endl;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
            dev_it !=  all_rc_devices_.end(); ++dev_it ) {
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 1 || dev.dev_typ_ == 2 ) {
            ofs << dev.dev_index_ << " " << dev.dev_n1_
                << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;
        }
    }
    ofs << "*END" << std::endl;

    return;
}


bool RCNetsData::write_spef_header( std::ofstream & ofs, std::string & design_name ) const
{
    ofs << "*SPEF \"1.0\"" << std::endl;
    ofs << "*DESIGN \"" << design_name << "\"" << std::endl;
    ofs << "*PROGRAM \"RandomNets\"" << std::endl;
    ofs << "*T_UNIT 1 NS" << std::endl;
    ofs << "*C_UNIT 1 FF" << std::endl;
    ofs << "*R_UNIT 1 OHM" << std::endl;
    ofs << std::endl;

    return true;
}

bool RCNetsData::write_spef_ports( std::ofstream & ofs ) const
{
    ofs << "*PORTS " << std::endl;
    for( std::map<std::string, char>::const_iterator port_it = all_ports_data_.begin();
            port_it !=  all_ports_data_.end(); ++port_it ) {
        ofs << port_it->first << " " << port_it->second << std::endl;
    }
    ofs << std::endl;

    return true;
}

bool RCNetsData::create_random_net( const std::string& net_name,
                                    const std::string& drv_node,
                                    const std::string& driver_celltype,
                                    const std::vector<std::string> & receivers,
                                    const std::vector<std::string> & receivers_celltypes,
                                    double total_cap, int max_layer_num )
{
    RandomRCNet* new_net = nullptr;

    if( have_net( net_name ) ) {
        std::cout << "Error: net " << net_name << "is already present" << std::endl;
        return false;
    }

    
    new_net = new RandomRCNet( net_name, drv_node, receivers, total_cap, max_layer_num, layer_data_);

    all_nets_data_.insert( std::map<std::string, RandomRCNet& >::value_type( net_name, *new_net ) );

    return true;
}


bool RCNetsData::create_random_net_scaled( const std::string& net_name,
                                    const std::string& drv_node,
                                    const std::string& driver_celltype,
                                    const std::vector<std::string> & receivers,
                                    const std::vector<std::string> & receivers_celltypes,
                                    double total_length, int max_layer_num,
                                    double total_cap, double cnear_cfar_ratio, double res_scale )
{
    RandomRCNet* new_net = nullptr;

    if( have_net( net_name ) ) {
        std::cout << "Error: net " << net_name << "is already present" << std::endl;
        return false;
    }

    new_net = new RandomRCNet( net_name, drv_node, receivers, total_length, max_layer_num, layer_data_, 
                                total_cap, cnear_cfar_ratio, res_scale);

    all_nets_data_.insert( std::map<std::string, RandomRCNet& >::value_type( net_name, *new_net ) );

    return true;
}

bool RCNetsData::have_net( const std::string & net_name) const
{
    std::map< std::string, RandomRCNet &>::const_iterator net_it =
        all_nets_data_.find( net_name );
    if( net_it != all_nets_data_.end() ) {
        return true;
    } else {
        return false;
    }
}


bool RCNetsData::add_new_port( const std::string & port_name, const char port_type )
{
    std::map<std::string, char>::const_iterator port_it = all_ports_data_.find( port_name );
    if( port_it != all_ports_data_.end() ) {
        std::cout << "Error: port " << port_name << " is already defined" << std::endl;
        return false;
    } else {
        all_ports_data_[port_name] = port_type;
        return true;
    }
}

bool RCNetsData::get_net_ref ( const std::string & net_name, RandomRCNet & net_data )
{
    std::map< std::string, RandomRCNet &>::iterator net_it =
        all_nets_data_.find( net_name );
    if (net_it != all_nets_data_.end()) {
        std::cout << "Found net " << net_name << std::endl;
        net_data = net_it->second;
        return true;
    } else {
        std::cout << "Did not find net " << net_name << std::endl;
        return false;
    }

}

bool RCNetsData::dump_spef( std::ofstream & ofs, std::string & design_name ) const
{
    write_spef_header( ofs, design_name );
    write_spef_ports( ofs );

    for ( std::map< std::string, RandomRCNet &>::const_iterator net_it = all_nets_data_.begin();
            net_it != all_nets_data_.end(); ++net_it ) {
        const RandomRCNet & my_net = net_it->second;
        my_net.dump_spef_dnet( ofs, 1.0, 1.0 );
    }

    return true;
}

bool RCNetsData::dump_spice( std::ofstream & ofs ) const
{

    for ( std::map< std::string, RandomRCNet &>::const_iterator net_it = all_nets_data_.begin();
            net_it != all_nets_data_.end(); ++net_it ) {
        const RandomRCNet & my_net = net_it->second;
        my_net.dump_spice_subckt_net( ofs, 1.0, 1.0 );
    }

    return true;
}




/******************************************************************************************
 * Test
 * ****************************************************************************************/

float rand_float(float from, float to) {
  float rand_scale = float(rand()) / float(RAND_MAX);

  return (to-from)*rand_scale + from;
}

double rand_cap( double from_val, double to_val )
{
    // we want to generate random cap values that have an exponential kind of probability
    // meaning that is is more likely to generate low cap values than high cap values
    // just like in a normal design the short nets outnumber the long ones

    if (from_val < 0.0) {
        from_val = 0.0;
    }
    double rand_uniform = double(rand()) / double(RAND_MAX) * 10.0;
    double rand_exp = pow(2.0, rand_uniform) / 1024.0;
    return (to_val-from_val)*rand_exp + from_val;
}


float get_max_lib_load( dctk::CellLib* cell_lib )
{
    // retrieve the maximum cap load from the library cells.
    // there must be a better way to iterate thru all the existing cells
    float scale_to_ff = cell_lib->get_scale_to_ff();

    std::vector<dctk::Cell*> driver_cells;

    driver_cells.push_back(cell_lib->get_cell("INVx2_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("INVx4_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("INVx13_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx2_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx4_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx12_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx24_ASAP7_75t_R"));

    float max_lib_load = 0.0;
    for( unsigned int d = 0; d<driver_cells.size(); ++d) {
        // Driver
        dctk::Cell* driver = driver_cells[d];
        dctk::CellPin* driver_output_pin = driver->get_output_pin();
        dctk::CellArc* driver_arc        = driver_output_pin->get_random_arc();

        // Find load ranges
        std::vector<float> load_values = driver_arc->get_load_values();
        int num_vals = load_values.size();
        float max_load_val = load_values[num_vals-1];

        if( max_load_val > max_lib_load ){
            max_lib_load = max_load_val;
        }
    }

    max_lib_load =  max_lib_load * scale_to_ff;
    return max_lib_load;
}

bool create_random_nets_stress( int num_nets, int max_num_receivers, double max_len, int max_layer_index,
                         RCNetsData & all_nets, dctk::CellLib* cell_lib, dctk::CircuitPtrVec& cktmgr, char* waveform )
{
   std::cout << "Stress function under construction" << std::endl;
   return false;
}

bool create_random_nets( int num_nets, int max_num_receivers, 
                         RCNetsData & all_nets, dctk::CellLib* cell_lib, dctk::CircuitPtrVec& cktmgr, char* waveform )
{
    int inst_num = 1;

    float scale_to_ps = cell_lib->get_scale_to_ps();
    float scale_to_ff = cell_lib->get_scale_to_ff();


    for( int i = 0; i<num_nets; i++ ) {
        std::string net_name = "net_" + std::to_string(i);

        //
        // Driver
        //
        dctk::Cell* driver = nullptr;

        float ramp_time = 51.0;
        
        float smallest_max_load = 0.0; // smallest max load of the NLDM tables
        float largest_min_load = std::numeric_limits<float>::infinity(); // largest min load of the NLDM tables

        // find a cell with an output pin
        dctk::CellPin* driver_output_pin = nullptr;
        dctk::CellPin* driver_input_pin = nullptr;
        while (driver_output_pin == nullptr or driver_input_pin == nullptr) {
            driver = cell_lib->get_random_cell();
            // cout << "Trying cell " << driver->get_name() << endl;
            driver_output_pin = driver->get_output_pin();

            // need to find an input pin that drives output pin
            dctk::CellArc* random_arc = driver_output_pin->get_random_arc();
            if (random_arc == nullptr) {
                driver_input_pin = nullptr;
            } else {
                driver_input_pin = random_arc->get_related_pin();

                // scale to ps (assumes input lib is in ns)
                ramp_time = random_arc->get_random_slew() * scale_to_ps ;

                // scale to ff (assumes input lib cap is in pf)
                random_arc->get_load_range(largest_min_load, smallest_max_load);
                smallest_max_load *= scale_to_ff;
                largest_min_load *= scale_to_ff;
            }
        }

        // Build driver node
        std::string driver_inst = "I" + std::to_string(inst_num);
        std::string driver_node = driver_inst + ":" + driver_output_pin->get_name();
        std::string driver_celltype = driver->get_name(); // needed later
        std::string driver_input_output_str =
            driver_inst + "/" + driver_input_pin->get_name() + "/" + driver_output_pin->get_name();
        inst_num++;


        //
        // Receivers
        //

        int num_receivers = rand() % max_num_receivers + 1;

        std::vector<std::string> receivers;
        std::vector<std::string> receivers_celltypes;
        std::string receiver_input_output_str;
        
        // total receiver pin cap
        float total_receiver_max_pin_cap = 0.0;
        float total_receiver_min_pin_cap = 0.0;
        float load_cap = std::numeric_limits<float>::quiet_NaN();

        for( int j=0; j<num_receivers; j++ ) {

            // find a random cell that has an input pin
            dctk::CellPin* receiver_input_pin = nullptr;
            dctk::CellPin* receiver_output_pin = nullptr;
            dctk::Cell* receiver = nullptr;

            dctk::CellArc* random_arc = nullptr;
            
            while (receiver_input_pin == nullptr or receiver_output_pin==nullptr) {
                receiver = cell_lib->get_random_cell();
                
                receiver_output_pin = receiver->get_output_pin();

                // need to find an input pin that drives output pin
                random_arc = receiver_output_pin->get_random_arc();
                if (random_arc == nullptr) {
                    receiver_input_pin = nullptr;
                } else {
                    receiver_input_pin = random_arc->get_related_pin();
                }
            }

            // get the min and max cap for the arc
            float receiver_arc_max_pin_cap;
            float receiver_arc_min_pin_cap;
            random_arc->get_min_max_pin_cap(receiver_arc_min_pin_cap, receiver_arc_max_pin_cap);

            // get the min and max cap for the input pin
            float receiver_input_min_pin_cap = receiver_input_pin->get_min_pin_cap();
            float receiver_input_max_pin_cap = receiver_input_pin->get_max_pin_cap();

            float max_pin_cap = std::max(receiver_arc_max_pin_cap, receiver_input_max_pin_cap);
            float min_pin_cap = std::min(receiver_arc_min_pin_cap, receiver_input_min_pin_cap);

            total_receiver_max_pin_cap += max_pin_cap * scale_to_ff;
            total_receiver_min_pin_cap += min_pin_cap * scale_to_ff;
            

            // build receiver node
            std::string receiver_inst = "I" + std::to_string(inst_num);
            std::string receiver_node = receiver_inst + ":" + receiver_input_pin->get_name();
            inst_num++;
            receivers.push_back( receiver_node );
            receivers_celltypes.push_back(receiver->get_name());

            // YAML file will use first pin processed
            if (receiver_input_output_str == "") {
                receiver_input_output_str = receiver_inst + "/" + receiver_input_pin->get_name() + "/" + receiver_output_pin->get_name();
            }

            // load interconnect will also be based on the first pin processed
            if (isnan(load_cap)) {
                load_cap = random_arc->get_random_load() * scale_to_ff;
            }
        }


        // now reduce the max load allowed by the amount of receiver pin caps
        smallest_max_load -= total_receiver_max_pin_cap;
        largest_min_load -= total_receiver_min_pin_cap;

        // generate a random net cap value in the range largest_min_load, smallest_max_load
        // such that lower loads have higher probability
        double net_load_cap = rand_cap( largest_min_load, smallest_max_load );

        // based on the chosen cap, identify an appropriate max layer
        int net_max_lyr = all_nets.layer_data_.get_guided_layer_index_by_cap( net_load_cap );

        all_nets.create_random_net( net_name, driver_node, driver_celltype, receivers,
                                    receivers_celltypes, net_load_cap, net_max_lyr );

        // add to a circuit library
        dctk::Circuit* c = new dctk::Circuit(net_name);

        // default to ramp if waveform not given
        std::string waveform_str("ramp");
        if (waveform) {
            waveform_str = std::string(waveform);
        }
        
        std::string input_waveform = waveform_str + " " + to_string(ramp_time) ;
        c->set_input_waveform(input_waveform);

        // driver
        c->set_driver(driver_input_output_str);
        c->set_driver_celltype(driver_celltype);

        // load (first receiver)
        c->set_load(receiver_input_output_str);
        c->set_load_celltype(receivers_celltypes[0]);

        // Start with a fixed load
        std::string load_interconnect = to_string(load_cap) + " 0 0";
        c->set_load_interconnect(load_interconnect.c_str());

	// add unused loads
	std::string unused_loads;
	for (int i = 1; i < num_receivers; i++) {
	  // create string such as I13/A inv32
	  if (i == 1) {
	    unused_loads = receivers[i] + " " + receivers_celltypes[i];
	  } else {
	    unused_loads = unused_loads + " " + receivers[i] + " " + receivers_celltypes[i];
	  }
        }
        c->set_unused_loads(unused_loads);

        // add to library
        cktmgr.push_back(c);

    }

    return true;
}



//
//
// Usage:
//
// gen_random_nets [options]
//
//   --liberty LIBERTY    Liberty model (input)
//   --circuits CIRCUITS  Test Circuits file (output)
//   --spef SPEF          SPEF with parasitics
//   --waveform WAVEFORM  waveform

//
// Format for circuits file (using YAML format)
//
// Circuits:
//   -
//     name: circuitname1 (also netname)
//     voltage_source:
//     driver: <instance/input/output>
//     driver_celltype: <celltype>
//     load:  <instance/input/output>
//     load_interconnect: <c1> <r> <c2>
//     receiver_instance: <instance>
//   -
//     name: circuitname2
//     ...
//


int main( int argc, char * const argv[] )
{
    // Liberty
    bool read_liberty_file = false;
    char* liberty_file = nullptr;
    dctk::CellLib* cell_lib = nullptr;
    int read_lib_retval = 0;

    // Test Circuits
    dctk::CircuitPtrVec circuitMgr;

    // dataset name
    std::string dataset_name;
    int n = 0;

    // waveform
    char *waveform = nullptr;

    // stress test
    bool stress = false;
    
    // get options
    int c;
    int option_index = 0;
    static struct option long_options[] = {
        {"liberty", required_argument, 0, 'l'},
        {"dataset", required_argument, 0, 'd'},
        {"number", required_argument, 0, 'n'},
        {"waveform", required_argument, 0, 'w'},
        {"stress", optional_argument, 0, 's'},
        {0,         0,                 0,  0 }
    };

    while  ((c = getopt_long(argc, argv, "l:d:n:psw:", long_options, &option_index))) {

        if (c == -1)
            break;

        switch (c) {
        case 'l':
            // liberty
            read_liberty_file = true;
            liberty_file = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(liberty_file, optarg);
            break;
        case 'd':
            // dataset name
            dataset_name = std::string(optarg);
            break;

        case 'n':
            // number of circuits
            n = atoi(optarg);
            break;

        case 's':
            // stress test
            stress = true;
            break;

        case 'w':
            // waveform
            waveform = (char*)malloc((strlen(optarg)+1) * sizeof(char));
            strcpy(waveform, optarg);
            break;

        case '?':
            break;


        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    // Read Liberty
    if (read_liberty_file) {
        printf("Reading Liberty file %s\n", liberty_file);
        read_lib_retval = read_liberty(liberty_file, cell_lib);
    } else {
        printf("Error:  Liberty option required!");
        exit(1);
    }
    if (read_lib_retval != 0) {
        printf("Error %d during Liberty processing.  Exiting.", read_lib_retval);
        if (liberty_file) {
            free(liberty_file);
        }
        exit(1);
    }

    //
    // Create the set of nets
    //
    RCNetsData all_nets;

    std::string tech_node = "7nm";
    LayerRCData layer_data( tech_node );

    // retrieve the maximum cap from the library
    float max_cap_lim = get_max_lib_load(cell_lib);

    // estimate the maximum length that can be driven for each metal layer
    // assuming that the max cap limit is for the top layer
    int max_num_layers = layer_data.get_num_layers();
    double top_layer_cap_scale = layer_data.get_layer_cap_per_len(max_num_layers-1);
    double max_total_len = max_cap_lim / top_layer_cap_scale;
    layer_data.get_guided_layer_lengths( max_total_len );

    // save the layer data
    all_nets.layer_data_ = layer_data;

    // seed random generator
    std::srand(std::time(nullptr));
    if (stress) {
        create_random_nets_stress( n, 4, 1000.0, 14, all_nets, cell_lib, circuitMgr, waveform );
    } else {
        create_random_nets( n, 4, all_nets, cell_lib, circuitMgr, waveform );
    }
 
    //
    // write out SPEF file
    //
    std::ofstream spef_fout;
    spef_fout.open(dataset_name + ".spef", std::ofstream::out);
    all_nets.dump_spef(spef_fout, dataset_name );
    spef_fout.close();

    //
    // write out circuit.yaml file using yaml-cpp
    //

    // format data
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "Circuits";
    emitter << YAML::BeginSeq;
    for (std::size_t i = 0; i < circuitMgr.size(); i++) {
        circuitMgr[i]->gen_yaml(emitter);
    }
    emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

    // write to file
    std::ofstream yaml_fout(dataset_name + ".circuits.yaml");
    yaml_fout << emitter.c_str() << std::endl;
    yaml_fout.close();

    //
    // memory cleanup
    //
    if (liberty_file) {
        free(liberty_file);
    }

    exit(0);
}


