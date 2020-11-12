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

int LayerRCData::get_guided_max_layer_index( double total_len ) const
{
    double best_del = 1.0e+10;
    int best_lyr_indx = 0;
    int num_lyrs = all_layer_data_.size();
    if( num_lyrs > 0 ) {
        for( int i=1; i<num_lyrs; i++ ) {
            double shape_cap = all_layer_data_[i].tot_cap_ * total_len;
            double shape_via = get_via_stack_res( -1, i );
            double shape_res = all_layer_data_[i].lyr_res_ * total_len;
            double lyr_del = shape_via * shape_cap + 0.5 * shape_res * shape_cap;
            if( lyr_del < best_del ) {
                best_del = lyr_del;
                best_lyr_indx = i;
            }
        }
        return best_lyr_indx;
    } else {
        return 0;
    }
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

bool RandomRCNet::populate_net_data( const std::string & net_name,
                                     const std::string & drv_node, const std::vector<std::string > & receivers,
                                     double total_length, int max_layer_index, const LayerRCData & layer_data )
{
    if( receivers.size() < 1 or max_layer_index < 0 or max_layer_index >= layer_data.get_num_layers() ) {
        return false;
    }

    net_index_ = net_name;
    pi_model_ = false;
    all_rc_devices_.clear();
    all_node_types_.clear();
    if( receivers.size() == 1 ) {
        double via_val = layer_data.get_via_stack_res( -1, max_layer_index );
        double shape_res = layer_data.get_shape_layer_res(max_layer_index, total_length);
        double shape_cap = layer_data.get_shape_layer_cap(max_layer_index, total_length);

        std::string gnd_node = "gnd";
        std::string new_node_1 = net_index_ + ":1";
        RCDevice drv_via( 1, 1, drv_node, new_node_1, via_val );
        all_rc_devices_.push_back( drv_via );
        RCDevice near_cap( 3, 1, new_node_1, gnd_node, shape_cap*0.5 );
        all_rc_devices_.push_back( near_cap );

        std::string new_node_2 = net_index_ + ":2";
        RCDevice shp_res( 2, 2, new_node_1, new_node_2, shape_res);
        all_rc_devices_.push_back( shp_res );
        RCDevice far_cap( 3, 2, new_node_2, gnd_node, shape_cap*0.5 );
        all_rc_devices_.push_back( far_cap );

        RCDevice rcv_via( 1, 3, new_node_2, receivers[0], via_val );
        all_rc_devices_.push_back( rcv_via );

        all_node_types_.insert( std::map<std::string, int>::value_type( gnd_node, 0 ) );  // gnd
        all_node_types_.insert( std::map<std::string, int>::value_type( drv_node, 1 ) );  // drv
        all_node_types_.insert( std::map<std::string, int>::value_type( new_node_1, 3 ) );
        all_node_types_.insert( std::map<std::string, int>::value_type( new_node_2, 3 ) );
        all_node_types_.insert( std::map<std::string, int>::value_type( receivers[0], 2 ) );  // rcvr

        return true;
    } else {
        double spine_len = (double(rand() % 50) + 50.5) / 100.0;
        double remaining_length = 1.0 - spine_len;
        std::vector<double> rcv_lengths;
        rcv_lengths.push_back( spine_len );
        double sum_extra_rcvr_len = 0.0;
        int num_receivers = receivers.size();
        for( int i=1; i<num_receivers; i++ ) {
            double rcvr_len = double(rand() % 100);
            rcv_lengths.push_back( rcvr_len );
            sum_extra_rcvr_len += rcvr_len;
        }
        for( int i=1; i<num_receivers; i++ ) {
            rcv_lengths[i] = rcv_lengths[i] / sum_extra_rcvr_len * remaining_length;
        }
        for( int i=0; i<num_receivers; i++ ) {
            rcv_lengths[i] = rcv_lengths[i] * total_length;
        }

        std::vector<double> spine_lengths;
        for(int i=0; i<num_receivers; i++) {
            double ith_seg_len = double(rand() % 100 + 1) / 100.0 * rcv_lengths[0];
            spine_lengths.push_back( ith_seg_len );
        }

        double drv_via_stack_res;
        std::vector<double>  spine_seg_res;
        std::vector<double>  spine_seg_cap;

        int wrc_code = layer_data.get_wire_segment_vrcdata( -1, max_layer_index,
                       spine_lengths, drv_via_stack_res, spine_seg_res, spine_seg_cap );

        if( wrc_code == 0 ) {
            return false;
        }

        std::string new_node_1 = net_index_ + ":1";
        RCDevice drv_via( 1, 1, drv_node, new_node_1, drv_via_stack_res );
        all_rc_devices_.push_back( drv_via );
        std::string gnd_node = "gnd";
        all_node_types_.insert( std::map<std::string, int>::value_type( gnd_node, 0 ) );  // gnd
        all_node_types_.insert( std::map<std::string, int>::value_type( drv_node, 1 ) );  // drv
        all_node_types_.insert( std::map<std::string, int>::value_type( new_node_1, 3 ) );  // internal
        int node_count = 1;
        int r_device_count = 2;
        int c_device_count = 1;
        int last_spine_node = 1;
        for( int i=1; i<num_receivers; i++) {
            double spine_res = spine_seg_res[i-1];
            double spine_cap = spine_seg_cap[i-1];

            std::string new_node_i = net_index_ + ":" + std::to_string(last_spine_node);
            node_count = node_count + 1;
            std::string new_node_ip1 = net_index_ + ":" + std::to_string(node_count);

            RCDevice shp_res( 2, r_device_count, new_node_i, new_node_ip1, spine_res);
            all_rc_devices_.push_back( shp_res );
            all_node_types_.insert( std::map<std::string, int>::value_type( new_node_ip1, 3 ) );
            r_device_count = r_device_count + 1;
            RCDevice far_cap( 3, c_device_count, new_node_ip1, gnd_node, spine_cap );
            all_rc_devices_.push_back( far_cap );
            c_device_count = c_device_count + 1;
            last_spine_node = node_count;

            int rcv_layer_indx = 0;
            if( max_layer_index > 0 ) {
                rcv_layer_indx = rand() % (max_layer_index);
            }
            double down_via = layer_data.get_via_stack_res( rcv_layer_indx-1, max_layer_index );
            double rcvr_res = layer_data.get_shape_layer_res(rcv_layer_indx, rcv_lengths[i]);
            double rcvr_cap = layer_data.get_shape_layer_cap(rcv_layer_indx, rcv_lengths[i]);

            node_count = node_count + 1;
            std::string new_node_ip2 = net_index_ + ":" + std::to_string(node_count);
            RCDevice down_rcv_via( 1, r_device_count, new_node_ip1, new_node_ip2, down_via );
            all_rc_devices_.push_back( down_rcv_via );
            r_device_count = r_device_count+1;
            all_node_types_.insert( std::map<std::string, int>::value_type( new_node_ip2, 3 ) );

            node_count = node_count + 1;
            std::string new_node_ip3 = net_index_ + ":" + std::to_string(node_count);
            RCDevice rcv_res_dev( 2, r_device_count, new_node_ip2, new_node_ip3, rcvr_res);
            all_rc_devices_.push_back( rcv_res_dev );
            all_node_types_.insert( std::map<std::string, int>::value_type( new_node_ip3, 3 ) );
            r_device_count = r_device_count+1;

            RCDevice rcvw1_cap_dev( 3, c_device_count, new_node_ip2, gnd_node, rcvr_cap/2.0);
            all_rc_devices_.push_back( rcvw1_cap_dev );
            c_device_count = c_device_count+1;
            RCDevice rcvw2_cap_dev( 3, c_device_count, new_node_ip3, gnd_node, rcvr_cap/2.0);
            all_rc_devices_.push_back( rcvw2_cap_dev );
            c_device_count = c_device_count+1;

            double rcvr_via = layer_data.get_via_stack_res( -1, rcv_layer_indx );
            RCDevice rcvr_rcv_via( 1, r_device_count, new_node_ip3, receivers[i], rcvr_via );
            all_rc_devices_.push_back( rcvr_rcv_via );
            r_device_count = r_device_count+1;
            all_node_types_.insert( std::map<std::string, int>::value_type( receivers[i], 2 ) ); // rcv
        }

        // add the first receiver at the end of the spine
        double spine_res = spine_seg_res[num_receivers-1];
        double spine_cap = spine_seg_cap[num_receivers-1];
        std::string new_node_i = net_index_ + ":" + std::to_string(last_spine_node);
        node_count = node_count + 1;
        std::string new_node_ip1 = net_index_ + ":" + std::to_string(node_count);
        RCDevice shp_res( 2, r_device_count, new_node_i, new_node_ip1, spine_res);
        all_rc_devices_.push_back( shp_res );
        all_node_types_.insert( std::map<std::string, int>::value_type( new_node_ip1, 3 ) );
        node_count = node_count + 1;
        r_device_count = r_device_count + 1;
        RCDevice far_cap( 3, c_device_count, new_node_ip1, gnd_node, spine_cap );
        all_rc_devices_.push_back( far_cap );
        c_device_count = c_device_count + 1;

        RCDevice rcvr_rcv_via( 1, r_device_count, new_node_ip1, receivers[0], drv_via_stack_res );
        all_rc_devices_.push_back( rcvr_rcv_via );
        r_device_count = r_device_count+1;
        all_node_types_.insert( std::map<std::string, int>::value_type( receivers[0], 2 ) ); // rcv

        return true;
    }

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
                                    double total_length, int max_layer_num,
                                    bool pimodel_net, float largest_min_load, float smallest_max_load)
{
    RandomRCNet* new_net = nullptr;

    if( have_net( net_name ) ) {
        std::cout << "Error: net " << net_name << "is already present" << std::endl;
        return false;
    }

    
    // until we have a net that has load cap within the limit, we keep trying
    while (new_net == nullptr) {
        new_net = new RandomRCNet( net_name, drv_node, receivers, total_length, max_layer_num, layer_data_, pimodel_net);

        // if net cap is larger than the characterization limit, delete and try again (after reducing the total length
        if (new_net->get_total_cap(1.0) > smallest_max_load) {
            std::cout << "Net load exceeds characterized limit.  Iterating:  smallest_max_load = " << smallest_max_load << "; total cap = " << new_net->get_total_cap(1.0) << std::endl;
            delete new_net;
            new_net = nullptr;
            total_length = total_length/2.0;
            continue;
        }

        if (new_net->get_total_cap(1.0) < largest_min_load) {
            std::cout << "Net load below characterized limit.  Iterating:  largest_min_load = " << largest_min_load << "; total cap = " << new_net->get_total_cap(1.0) << std::endl;
            delete new_net;
            new_net = nullptr;
            total_length = total_length*1.5;
            continue;
        }
            
    }
                    
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

// stress test version of create_random_nets()
bool create_random_nets_stress( int num_nets, int max_num_receivers, double max_len, int max_layer_index,
                         RCNetsData & all_nets, dctk::CellLib* cell_lib, dctk::CircuitPtrVec& cktmgr, bool pimodels, char* waveform )
{
    int max_len_int = int( ceil(max_len) );
    
    int test_per_stratus = num_nets;

    int inst_num = 1;

    float scale_to_ps = cell_lib->get_scale_to_ps();
    float scale_to_ff = cell_lib->get_scale_to_ff();

    std::vector<dctk::Cell*> driver_cells;

    driver_cells.push_back(cell_lib->get_cell("INVx2_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("INVx4_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("INVx13_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx2_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx4_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx12_ASAP7_75t_R"));
    driver_cells.push_back(cell_lib->get_cell("BUFx24_ASAP7_75t_R"));

    std::vector<dctk::Cell*> receiver_cells;
    std::vector<float> receiver_loads;
    receiver_cells.push_back(cell_lib->get_cell("INVx2_ASAP7_75t_R"));
    receiver_loads.push_back(4.0 * 0.00119678); //FO4 

    int i = 0; // Curent net 
 
    for(unsigned d = 0; d<driver_cells.size(); ++d) {
      for(unsigned r = 0; r<receiver_cells.size(); ++r) {

        // Driver
        dctk::Cell* driver = driver_cells[d];
        dctk::CellPin* driver_output_pin = driver->get_output_pin();
        dctk::CellPin* driver_input_pin  = driver->get_random_input_pin(); 
        dctk::CellArc* driver_arc        = driver_output_pin->get_random_arc();

        // Receiver
        std::vector<std::string> receivers;
        std::vector<std::string> receivers_celltypes;
        std::string receiver_input_output_str;
 
        float       receiver_load = receiver_loads[r] * scale_to_ff;  
        dctk::Cell* receiver = receiver_cells[r];
        dctk::CellPin* receiver_output_pin = receiver->get_output_pin();
        dctk::CellPin* receiver_input_pin  = receiver->get_random_input_pin();

        // Find input slew and load ranges
        std::vector<float> slew_values = driver_arc->get_slew_values();
        std::vector<float> load_values = driver_arc->get_load_values();
      
        //-2 so we skip the largest slew and largest load 
        for(unsigned s=0; s<slew_values.size()-2; ++s) {
          for(unsigned l=0; l<load_values.size()-2; ++l) {
            float s_min = slew_values[s] * scale_to_ps;
            float s_max = slew_values[s+1] * scale_to_ps;

            float l_min = load_values[l] * scale_to_ff;
            float l_max = load_values[l+1] * scale_to_ff;

            std::cout << "\n Slew [" <<  s_min << ", " << s_max << "] Load [" << l_min << ", " << l_max << "]" << std::endl;

            for (int n=0; n<test_per_stratus; ++n) {
              // Build driver node
              std::string driver_inst = "I" + std::to_string(inst_num);
              std::string driver_node = driver_inst + ":" + driver_output_pin->get_name();
              std::string driver_celltype = driver->get_name(); // needed later
              std::string driver_input_output_str =
                  driver_inst + "/" + driver_input_pin->get_name() + "/" + driver_output_pin->get_name();
              inst_num++;
      
              std::cout << "Driver Inst: " << driver_inst << std::endl;
              std::cout << "Driver Node: " << driver_node << std::endl;
              std::cout << "Driver CellType: " << driver_celltype << std::endl;
              std::cout << "Driver InputOutputString: " << driver_input_output_str << std::endl;
         
              // Build receiver node
              std::string receiver_inst = "I" + std::to_string(inst_num);
              std::string receiver_node = receiver_inst + ":" + receiver_input_pin->get_name();
              receiver_input_output_str = receiver_inst + "/" + receiver_input_pin->get_name() + "/" + receiver_output_pin->get_name();
              inst_num++;
      
              receivers.push_back( receiver_node );
              receivers_celltypes.push_back(receiver->get_name());
      
              std::cout << "Receiver Inst: " << receiver_inst << std::endl;
              std::cout << "Receiver Node: " << receiver_node << std::endl;
              std::cout << "Receiver CellType: " << receivers_celltypes[receivers_celltypes.size()-1] << std::endl;
              std::cout << "Receiver InputOutputString: " << receiver_input_output_str << std::endl;

              // Random values
              float ramp_time        = rand_float(s_min, s_max);
              float total_cap        = rand_float(l_min, l_max);
              float cnear_cfar_ratio = rand_float(0.2, 1.5);
              float res_scale        = rand_float(1.0, 2.0);

              std::cout << "Ramp Time: " << ramp_time << std::endl;
              std::cout << "Total Cap: " << total_cap << std::endl;
              std::cout << "Cn/Cf Rat: " << cnear_cfar_ratio << std::endl;
              std::cout << "Res Scale: " << res_scale << std::endl;

              // Net
              std::string net_name = "net_" + std::to_string(i);
              ++i;

              std::cout << "Net Name: " << net_name << std::endl;

              double net_len =  double(rand() % max_len_int + 1);
              int net_max_lyr = rand() % max_layer_index + 1;

              cout << "Creating random net with " << 1 << " rcvrs " << net_len << " length "
                           << net_max_lyr << " max layer" << std::endl;


              all_nets.create_random_net_scaled( net_name, driver_node, driver_celltype, receivers,
                                          receivers_celltypes, net_len, net_max_lyr,
                                          total_cap, cnear_cfar_ratio, res_scale);

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
              std::string load_interconnect = to_string(receiver_load) + " 0 0";
              c->set_pimodel_interconnect(load_interconnect);

              // add to library
              cktmgr.push_back(c); 
            }
          }
        } 
      }
    }

    return true;
}

bool create_random_nets( int num_nets, int max_num_receivers, double max_len, int max_layer_index,
                         RCNetsData & all_nets, dctk::CellLib* cell_lib, dctk::CircuitPtrVec& cktmgr, bool pimodels, char* waveform )
{
    int max_len_int = int( ceil(max_len) );

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
        if (pimodels) {
            num_receivers = 1;
        }

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
        assert(smallest_max_load>0.0);
        largest_min_load -= total_receiver_min_pin_cap;

        double net_len =  double(rand() % max_len_int + 1);
        int net_max_lyr = rand() % max_layer_index + 1;

        cout << "Creating random net with " << num_receivers << " rcvrs " << net_len << " length "
             << net_max_lyr << " max layer" << std::endl;

        all_nets.create_random_net( net_name, driver_node, driver_celltype, receivers,
                                    receivers_celltypes, net_len, net_max_lyr, pimodels, largest_min_load, smallest_max_load );

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
        c->set_pimodel_interconnect(load_interconnect.c_str());

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
//     name: circuitname1
//     voltage_source:
//     driver: <celltype/input/output>
//     load:  <celltype/input> <instance/input>
//     load_interconnect: <c1> <r> <c2>
//     driver_instance:  <instance>
//     receiver_instance: <instance>
//     spef: netname
//   -
//     name: circuitname2
//     ...
//


int main( int argc, char * const argv[] )
{
     RCNetsData all_nets;

    std::string tech_node = "7nm";
    LayerRCData layer_data( tech_node );
    all_nets.layer_data_ = layer_data;

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

    // pi models
    bool pimodels = false;

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
        {"pimodels", optional_argument, 0, 'p'},
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

        case 'p':
            // pi models only
            pimodels = true;
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

    // seed random generator
    std::srand(std::time(nullptr));
    if (stress) {
        create_random_nets_stress( n, 4, 1000.0, 14, all_nets, cell_lib, circuitMgr, pimodels, waveform );
    } else {
        create_random_nets( n, 4, 1000.0, 14, all_nets, cell_lib, circuitMgr, pimodels, waveform );
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


