#include "RandomRCNet.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
# include <cmath>

using namespace std;

LayerRCData::LayerRCData( std::string & tech_node )
{
     res_scale_ = 1.0;
     cap_scale_ = 1.0e-15;
     len_scale_ = 1.0e-6;

     if( tech_node == "7nm" ){
	     create_7nm_data();
	     return;
     } else if( tech_node == "14nm" ){
             create_14nm_data();
             return;
     } else if( tech_node == "20nm" ){
             create_20nm_data();
             return;
     } else if( tech_node == "28nm" ){
             create_28nm_data();
             return;
     } else if( tech_node == "40nm" ){
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
     for (int i=0; i<16; i++){
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
     for (int i=0; i<14; i++){
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
     for (int i=0; i<13; i++){
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
     for (int i=0; i<12; i++){
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
     for (int i=0; i<11; i++){
         VRCData new_layer( i, via[i], res[i], cap[i] );
         all_layer_data_.push_back( new_layer );
     }
}

int LayerRCData::get_num_layers() const 
{ return all_layer_data_.size(); }

int LayerRCData::get_rand_max_layer_index() const
{
    int num_lyrs = all_layer_data_.size();
    if( num_lyrs > 0 ){
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
    if( num_lyrs > 0 ){
        for( int i=1; i<num_lyrs; i++ ){
	    double shape_cap = all_layer_data_[i].tot_cap_ * total_len;
	    double shape_via = get_via_stack_res( -1, i );
	    double shape_res = all_layer_data_[i].lyr_res_ * total_len;
	    double lyr_del = shape_via * shape_cap + 0.5 * shape_res * shape_cap;
	    if( lyr_del < best_del ){
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
    if( start_lyr > end_lyr ){
        start_lyr = second_layer_index;
	end_lyr = first_layer_index;
    }
    double via_stack = 0.0;
    for(int i=start_lyr+1; i<=end_lyr; i++){
	via_stack = via_stack + all_layer_data_[i].via_res_;
    }
    return via_stack;
}

double LayerRCData::get_shape_layer_res( int layer_index, double layer_length ) const
{
    if( layer_index < 0 || layer_index >= all_layer_data_.size() ){
	return 0.0;
    } else {
	double shape_res = all_layer_data_[layer_index].lyr_res_ * layer_length;
	return shape_res;
    }
}

double LayerRCData::get_shape_layer_cap( int layer_index, double layer_length ) const
{
    if( layer_index < 0 || layer_index >= all_layer_data_.size() ){
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
		    sl_it != total_segment_lengths.end(); ++sl_it ){
	double seg_length = *sl_it;
	double shape_res = get_shape_layer_res(segment_layer_index, seg_length);
	double shape_cap = get_shape_layer_cap(segment_layer_index, seg_length);
	seg_res.push_back( shape_res );
	seg_cap.push_back( shape_cap );
    }
    if( via_stack_res <= 0.0 ){ return 0; }
    else{ return 1; }
}


bool RandomRCNet:: populate_net_data( const std::string & net_name,
                        const std::string & drv_node, const std::vector<std::string > & receivers,
                        double total_length, int max_layer_index, const LayerRCData & layer_data )
{
    if( receivers.size() < 1 or max_layer_index < 0 or max_layer_index >= layer_data.get_num_layers() ){
	return false;
    }

    net_index_ = net_name;
    all_rc_devices_.clear();
    all_node_types_.clear();
    if( receivers.size() == 1 ){
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
	for( int i=1; i<num_receivers; i++ ){
 	    double rcvr_len = double(rand() % 100);
	    rcv_lengths.push_back( rcvr_len );
	    sum_extra_rcvr_len += rcvr_len;
	}
	for( int i=1; i<num_receivers; i++ ){
	    rcv_lengths[i] = rcv_lengths[i] / sum_extra_rcvr_len * remaining_length;
	}
	for( int i=0; i<num_receivers; i++ ){
            rcv_lengths[i] = rcv_lengths[i] * total_length;
	}

	std::vector<double> spine_lengths;
	for(int i=0; i<num_receivers; i++){
	    double ith_seg_len = double(rand() % 100 + 1) / 100.0 * rcv_lengths[0];
	    spine_lengths.push_back( ith_seg_len );
	}

	double drv_via_stack_res;
	std::vector<double>  spine_seg_res;
	std::vector<double>  spine_seg_cap;

	int wrc_code = layer_data.get_wire_segment_vrcdata( -1, max_layer_index,
                        spine_lengths, drv_via_stack_res, spine_seg_res, spine_seg_cap );
        
	if( wrc_code == 0 ){
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
	for( int i=1; i<num_receivers; i++){
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
	    if( max_layer_index > 0 ){ rcv_layer_indx = rand() % (max_layer_index); }
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
                    rcv_it != rcv_nodes_.end(); ++rcv_it){
        const std::string & rcv_node = *rcv_it;
	ofs << " " << rcv_node;
    }
    ofs << std::endl;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
		    dev_it !=  all_rc_devices_.end(); ++dev_it ){
	const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 1 ){ 
	    ofs << "RV" << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;	
	} else if( dev.dev_typ_ == 2 ){
	    ofs << "RW" << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;
        } else if( dev.dev_typ_ == 3 ){
	    ofs << "CG" << dev.dev_index_ << " " << dev.dev_n1_ << " 0 " << dev.dev_val_*cap_scale << std::endl;
        }
    }

    ofs << ".ENDS " << net_index_ << std::endl;

    return;
}

void RandomRCNet::dump_spef_dnet( std::ofstream & ofs, double res_scale, double cap_scale ) const
{
    double tot_cap = 0.0;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
                    dev_it !=  all_rc_devices_.end(); ++dev_it ){
	const RCDevice & dev = *dev_it;
	if( dev.dev_typ_ == 3 ){
	    tot_cap += dev.dev_val_ * cap_scale;
	}
    }
    
    ofs << std::endl;
    ofs << "*D_NET " << net_index_ << " " << tot_cap << std::endl;
    ofs << "*CONN" << std::endl;
    int port_count = 0;
    for ( std::map< std::string, int >::const_iterator port_it = all_node_types_.begin();
                   port_it != all_node_types_.end(); ++port_it){
        if( port_it->second == 1 ){
            ofs << "*I *" << port_it->first << " O" << std::endl;
        } else if(  port_it->second == 2 ){
	    ofs << "*I *" << port_it->first << " I" << std::endl;
	}
    }

    ofs << "*CAP" << std::endl;
    int cap_count = 0;
     for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
                    dev_it !=  all_rc_devices_.end(); ++dev_it ){
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 3 ){
            ofs << "*" << dev.dev_index_ << " " << dev.dev_n1_ << " " << dev.dev_val_*cap_scale << std::endl;
        }
    }

    ofs << "*RES" << std::endl;
    int res_count = 0;
    for ( std::list< RCDevice >::const_iterator dev_it = all_rc_devices_.begin();
                    dev_it !=  all_rc_devices_.end(); ++dev_it ){
        const RCDevice & dev = *dev_it;
        if( dev.dev_typ_ == 1 || dev.dev_typ_ == 2 ){
            ofs << "*" << dev.dev_index_ << " " << dev.dev_n1_ << " " 
		    << " " << dev.dev_n2_ << " " << dev.dev_val_*res_scale << std::endl;
        } 
    }
    ofs << "*END" << std::endl;

    return;
}


bool RCNetsData::write_spef_header( std::ofstream & ofs, std::string & design_name ) const
{
	ofs << "*SPEF 1.0" << std::endl;
	ofs << "*Design " << design_name << std::endl;
        ofs << "*PROGRAM RandomNets" << std::endl;
        ofs << "*T_UNIT 1n" << std::endl;
	ofs << "*C_UNIT 1f" << std::endl;
	ofs << "*R_UNIT 1" << std::endl;
	ofs << std::endl;

	return true;
}

bool RCNetsData::write_spef_ports( std::ofstream & ofs ) const
{
        ofs << "*PORTS " << std::endl;
	for( std::map<std::string, char>::const_iterator port_it = all_ports_data_.begin();
			port_it !=  all_ports_data_.end(); ++port_it ){
		ofs << port_it->first << " " << port_it->second << std::endl;
	}
	ofs << std::endl;

	return true;
}
		



bool RCNetsData::create_random_net( const std::string & net_name,
                        const std::string & drv_node,
                        const std::vector<std::string> & receivers,
                        double total_length, int max_layer_num )
{
   if( have_net( net_name ) ){
	   std::cout << "Error: net " << net_name << "is already present" << std::endl;
	   return false;
   }

   RandomRCNet *new_net = new RandomRCNet( net_name, drv_node, receivers, total_length, max_layer_num, layer_data_);
   all_nets_data_.insert( std::map<std::string, RandomRCNet& >::value_type( net_name, *new_net ) );

   return true;
}

bool RCNetsData::have_net( const std::string & net_name) const
{
	std::map< std::string, RandomRCNet &>::const_iterator net_it = 
		all_nets_data_.find( net_name );
	if( net_it != all_nets_data_.end() ){
		return true;
	} else {
		return false;
	}
}

bool RCNetsData::add_new_port( const std::string & port_name, const char port_type )
{
	std::map<std::string, char>::const_iterator port_it = all_ports_data_.find( port_name );
	if( port_it != all_ports_data_.end() ){
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
	if (net_it != all_nets_data_.end()){
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
		    net_it != all_nets_data_.end(); ++net_it ){
	const RandomRCNet & my_net = net_it->second;
	my_net.dump_spef_dnet( ofs, 1.0, 1.0 );
    }

    return true;
}

bool RCNetsData::dump_spice( std::ofstream & ofs ) const
{

    for ( std::map< std::string, RandomRCNet &>::const_iterator net_it = all_nets_data_.begin();
                    net_it != all_nets_data_.end(); ++net_it ){
        const RandomRCNet & my_net = net_it->second;
        my_net.dump_spice_subckt_net( ofs, 1.0, 1.0 );
    }

    return true;
}
	


/******************************************************************************************
 * Test
 * ****************************************************************************************/

bool create_random_nets( int num_nets, int max_num_receivers, double max_len, int max_layer_index,
	       RCNetsData & all_nets )
{
    int max_rand_int = num_nets * (max_num_receivers + 1) + 1000;
    int max_len_int = int( ceil(max_len) );
	   
    for( int i = 0; i<num_nets; i++ ){
        std::string net_name = "net_" + std::to_string(i);
        int drv_inst_num = rand() % max_rand_int + 1;
        std::string drv_name = "I" + std::to_string(drv_inst_num) + ":Z";

        int num_receivers = rand() % max_num_receivers + 1;
	std::vector<std::string> receivers;
        for( int j=0; j<num_receivers; j++ ){
	    int rcv_inst_num = rand() % max_rand_int + 1;
	    int rcv_pin_num =  rand() % 4 + 1;
	    std::string rcv_name = "I" + std::to_string(rcv_inst_num) + ":A" + std::to_string(rcv_pin_num);
            receivers.push_back( rcv_name );
	}
	double net_len =  double(rand() % max_len_int + 1);
	int net_max_lyr = rand() % max_layer_index + 1;

	cout << "Creating random net with " << num_receivers << " rcvrs " << net_len << " length " 
		<< net_max_lyr << " max layer" << std::endl;
	all_nets.create_random_net( net_name, drv_name, receivers, net_len, net_max_lyr );

    }

    return true;
}


int main( int argc, char * const argv[] )
{
    RCNetsData all_nets;

    std::string tech_node = "7nm";
    LayerRCData layer_data( tech_node );
    all_nets.layer_data_ = layer_data;

    /*
    std::string net_name = "net_1";
    std::string drv_name = "I361:Z";
    std::vector< std::string > receivers;
    std::string snk1 = "I382:A1";
    std::string snk2 = "I2890:B2";
    std::string snk3 = "I20:A2";
    receivers.push_back( snk1 );
    receivers.push_back( snk2 );
    receivers.push_back( snk3 );

    all_nets.create_random_net( net_name, drv_name, receivers, 1000.0, 6 );

    std::map< std::string, RandomRCNet &>::iterator net_it =
                               all_nets.all_nets_data_.find( net_name );

    RandomRCNet & new_net = net_it->second;
*/
    create_random_nets( 10, 4, 2000.0, 14, all_nets );

    std::ofstream  ofs;
    ofs.open( "cuc.out",  std::ofstream::out); 
    // new_net.dump_spef_dnet( ofs, 1.0, 1.0 );
    //
    std::string design = "test_nets";
    all_nets.dump_spef( ofs, design ); 
    ofs.close();

    exit(0);
}


