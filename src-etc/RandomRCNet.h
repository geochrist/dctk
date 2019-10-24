#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>

class VRCData
{
public:
	VRCData(): lyr_index_(0), via_res_(0.0), lyr_res_(0.0), tot_cap_(0.0) {}
	VRCData(int indx, double via_res, double layer_res, double total_cap)
	{
		lyr_index_ = indx;
		via_res_ = via_res;
		lyr_res_ = layer_res;
		tot_cap_ = total_cap;
	}

	VRCData( const VRCData & other )
	{
		lyr_index_ = other.lyr_index_;
		via_res_ = other.via_res_;
		lyr_res_ = other.lyr_res_;
		tot_cap_ = other.tot_cap_;
	}
	const VRCData & operator = (const VRCData & other)
	{
		lyr_index_ = other.lyr_index_;
		via_res_ = other.via_res_;
		lyr_res_ = other.lyr_res_;
		tot_cap_ = other.tot_cap_;
		return *this;
	}

	// Data: it is per unit length for layer R and C (the unit is micron)
	int    lyr_index_;	// the index of this layer starting from 0 for M0
	double via_res_;	// via resistance to lower layer
	double lyr_res_;	// layer resistance per unit length for min width
	double tot_cap_;	// layer total capacitance per unit length for min width
};


class LayerRCData
{
public:
	LayerRCData() : all_layer_data_(), res_scale_(1.0), cap_scale_(1.0e-15), len_scale_(1.0e-6) {}

	// create the layer R and C data. The tech node must be one of: "7nm", "14nm", "20nm", "28nm", "40nm" 
	LayerRCData( std::string & tech_node );

	LayerRCData( const LayerRCData & other ) : 
		all_layer_data_(other.all_layer_data_), res_scale_(other.res_scale_),
                cap_scale_(other.cap_scale_), len_scale_(other.len_scale_) {}

	const LayerRCData & operator = (const LayerRCData & other )
	{
		all_layer_data_.clear();
		for (std::vector< VRCData >::const_iterator vrc_it = other.all_layer_data_.begin();
				vrc_it != other.all_layer_data_.end(); ++vrc_it ){
                     VRCData new_layer( *vrc_it );
                     all_layer_data_.push_back( new_layer );
                }
		res_scale_ = other.res_scale_;
                cap_scale_ = other.cap_scale_; 
		len_scale_ = other.len_scale_;
		return *this;
	}

	void create_7nm_data();
	void create_14nm_data();
	void create_20nm_data();
	void create_28nm_data();
	void create_40nm_data();

	// get the number of layers allowed for the given technology
	int get_num_layers() const;

	// get a random max layer index for the given technology >= 1 & < num_layers  
	int get_rand_max_layer_index() const;

	// get a guided random max layer index for the given technology
	// we choose the max layer to be suitable more or less for the given length
	int get_guided_max_layer_index( double total_len ) const;

	// get a resistance and capacitance for a metal shape of a given length and layer
	double get_shape_layer_res( int layer_index, double layer_length ) const;
	double get_shape_layer_cap( int layer_index, double layer_length ) const;

	// get the via stack resistance between two layers: for example the via stack
	// from M3 to M5 includes the via res from M3 to M4 and the via res from M4 to M5
	// which means we only take the via res from layers M4 and M5
	// If we need the via stack res from transistor level (stdcell pin) all the way to 
	// M5 we will indicate layer indices -1 to 5 (or 5 to -1)
	double get_via_stack_res( int first_layer_index, int second_layer_index ) const;

	// get the full info for one wire shape between two nodes: N1 and N2
	// where N1 is the "parent" node. The info contains a via stack resistance from
	// the given layer to the parent node layer, a shape resistance and a shape cap
	// The return value is 0 for same layer connection (i.e. when via res is = 0 because 
	// parent_layer_index = shape_layer_index) or 1 otherwise (via res != 0)
	// Please note!!! that we return only one via stack res on one of the two end
	// nodes of the shape.
	// To make things more flexible (or complicated) we can give a number of sections
	// for this wire shape in case we want to add connections to other shapes
	// For each length indicated in total_segment_lengths we get alayer r and c  
	int get_wire_segment_vrcdata( int parent_layer_index, int segment_layer_index, 
			const std::vector<double> & total_segment_lengths, 
			double & via_stack_res, 
			std::vector<double> & seg_res, 
			std::vector<double> & seg_cap ) const;

	// Data
	std::vector< VRCData > all_layer_data_;
	double res_scale_; // res is ohm per micron, default 1.0
	double cap_scale_; // cap is fF per micron, default 1.0e-15
	double len_scale_; // len is in microns, default 1.0e-6
};


/*************************************************************************
 * RCDevice *
 * Basic res or cap device 
 * **********************************************************************/
class RCDevice
{
public:
	RCDevice() : dev_typ_(0), dev_index_(0),
                dev_n1_("undef"), dev_n2_("undef"), dev_val_(0.0) {}
	RCDevice( int dev_type, int dev_index, std::string n1, std::string n2, double val )
		: dev_typ_(dev_type), dev_index_(dev_index),
		dev_n1_(n1), dev_n2_(n2), dev_val_(val) {}
	RCDevice( const RCDevice & other )
	{
		dev_typ_ = other.dev_typ_;
		dev_index_ = other.dev_index_;
		dev_n1_ = other.dev_n1_;
	       	dev_n2_ = other.dev_n2_;
       		dev_val_ = other.dev_val_;	       
	}
	const RCDevice & operator = ( const RCDevice & other )
        {
                dev_typ_ = other.dev_typ_;
                dev_index_ = other.dev_index_;
                dev_n1_ = other.dev_n1_;
                dev_n2_ = other.dev_n2_;
                dev_val_ = other.dev_val_;
        	return *this;
	}

	void dump_device( std::ofstream & ofs )
        {
	   if( dev_typ_ == 0 ){
	     return;
	   } else {
              if( dev_typ_ == 1 ||  dev_typ_ == 2 ){
		ofs << dev_index_ << " " << dev_n1_ << " " << dev_n2_ << " " << dev_val_ << std::endl;
	      } else {
		// we only have caps to gnd
		ofs << dev_index_ << " " << dev_n1_ << " " << dev_val_ << std::endl;
	      }
	   }
	   return;
	}

	// Data
	int dev_typ_;	// 0 undef, 1 via, 2 layer_res, 3 cap to gnd (n2=0)
	int dev_index_;	// some unique index for this device
	std::string dev_n1_;
	std::string dev_n2_;
	double dev_val_;
};	

/************************************************************************
 * RandomRCNet *
 * Create a simple multi-layer multi-receiver net (only one driver)
 * The num_receivers must be 1 or more. We assign a random length to the first
 * receiver such that it is more than half of the given net length
 * We create a "shape" for this receiver on the max layer allowed for this net
 * and we break this shape into segments for connecting the remaining receivers.
 * For each remaining receiver we create a shape on a lower layer with only one segment
 * ***********************************************************************/  
class RandomRCNet
{
public:
	RandomRCNet() : net_index_(), all_rc_devices_() {}
	RandomRCNet( const std::string & net_name, const std::string & drv_node, 
			const std::vector<std::string > & receivers, 
			double total_length, int max_layer_num, 
			const LayerRCData & layer_data ) 
	{
		net_index_ = net_name;
		drv_node_ = drv_node;
		for( std::vector< std::string >::const_iterator rcv_it = receivers.begin();
				rcv_it != receivers.end(); ++rcv_it){
			std::string new_rcv = *rcv_it;
			rcv_nodes_.push_back( new_rcv );
		}
		populate_net_data( net_name, drv_node, receivers, total_length, 
				max_layer_num, layer_data );
	}

	RandomRCNet( const RandomRCNet & other )
	{
		net_index_ = other.net_index_;
		drv_node_ = other.drv_node_;
		for( std::vector<std::string>::const_iterator rcv_it = other.rcv_nodes_.begin();
				rcv_it != other.rcv_nodes_.end(); ++rcv_it){
			std::string rcv_node = *rcv_it;
			rcv_nodes_.push_back( rcv_node );
		}
		for( std::list< RCDevice >::const_iterator dev_it = other.all_rc_devices_.begin();
				dev_it != other.all_rc_devices_.end(); ++dev_it ){
			RCDevice new_dev = *dev_it;
			all_rc_devices_.push_back( new_dev );
		}
		for( std::map< std::string, int >::const_iterator nod_it = other.all_node_types_.begin();
				nod_it != other.all_node_types_.end(); ++nod_it){
			all_node_types_.insert( std::map<std::string, int>::value_type( (*nod_it).first, (*nod_it).second ) );
		}
	}
	const RandomRCNet & operator = ( const RandomRCNet & other )
        {
		net_index_ = other.net_index_;
                drv_node_ = other.drv_node_;
                for( std::vector<std::string>::const_iterator rcv_it = other.rcv_nodes_.begin();
                                rcv_it != other.rcv_nodes_.end(); ++rcv_it){
                        std::string rcv_node = *rcv_it;
                        rcv_nodes_.push_back( rcv_node );
                }
                for( std::list< RCDevice >::const_iterator dev_it = other.all_rc_devices_.begin();
                                dev_it != other.all_rc_devices_.end(); ++dev_it ){
                        RCDevice new_dev = *dev_it;
                        all_rc_devices_.push_back( new_dev );
                }
                for( std::map< std::string, int >::const_iterator nod_it = other.all_node_types_.begin();
                                nod_it != other.all_node_types_.end(); ++nod_it){
                        all_node_types_.insert( std::map<std::string, int>::value_type( (*nod_it).first, (*nod_it).second ) );
                }
		return *this;
        }

        ~RandomRCNet()
	{
		rcv_nodes_.clear();
		all_rc_devices_.clear();
		all_node_types_.clear();
	}

	// method that creates the actual RC devices for this net
	bool populate_net_data( const std::string & net_name, 
			const std::string & drv_node, const std::vector<std::string > & receivers,
		       	double total_length, int max_layer_num, const LayerRCData & layer_data );

	void dump_spice_subckt_net( std::ofstream & ofs, double res_scale, double cap_scale ) const;

	void dump_spef_dnet( std::ofstream & ofs, double res_scale, double cap_scale ) const;

	// Data:
	std::string net_index_;		//unique net index
	std::string drv_node_;		// driver node name
	std::vector<std::string> rcv_nodes_;  // receiver names
	std::list< RCDevice > all_rc_devices_; 	// all the devices
	std::map< std::string, int > all_node_types_;	// map node number to node type
			// node 0 is always ground. The types can be 0 = ground
			// 1 = driver, 2 = receiver, 3 = internal node
};


/********************************************************************************
 * RCNetsData
 * Keep track of all the nets created such that at the end the user can dump out the 
 * spef data for all nets
 * ATTENTION!! the spef net names are not mapped to integers
 *******************************************************************************/
class RCNetsData {
public:
	RCNetsData() : all_nets_data_(), all_ports_data_() {}
        ~RCNetsData()
	{
	    for ( std::map< std::string, RandomRCNet &>::const_iterator net_it = all_nets_data_.begin();
                    			net_it != all_nets_data_.end(); ++net_it ){
		RandomRCNet * my_net = &( net_it->second );
		delete [] my_net;
	    }
	    all_nets_data_.clear();
	    all_ports_data_.clear();
	}
	
	// create a new net, all the drv and receiver instance pin or port name(s) must 
	// be specified by the user. The net is added to the all nets map
	bool create_random_net( const std::string & net_name,
                        const std::string & drv_node, 
			const std::vector<std::string > & receivers,
                        double total_length, int max_layer_num );

	bool add_new_port( const std::string & port_name, const char port_type );

	bool have_net( const std::string & net_name) const;

	// return a non-const reference to the data. The user must check if net exists first!
	bool get_net_ref ( const std::string & net_name, RandomRCNet & net_data );

	bool write_spef_header( std::ofstream & ofs, std::string & design_name ) const;
	bool write_spef_ports( std::ofstream & ofs ) const;
	bool dump_spef( std::ofstream & ofs, std::string & design_name ) const;
	bool dump_spice(  std::ofstream & ofs ) const;

	// Data
	LayerRCData layer_data_;
	std::map<std::string, RandomRCNet &> all_nets_data_;   // map net_name to net data
	std::map<std::string, char> all_ports_data_;		// map port name to port type
};
