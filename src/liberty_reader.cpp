// Liberty reader routines



#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "si2dr_liberty.h"
#include "CellLib.hpp"
#include "Cell.hpp"
#include "CellPin.hpp"
#include "CellArc.hpp"

class CellArc;

int read_liberty(char *filename, dctk::CellLib*& cell_lib) {

    si2drErrorT err;
    si2drGroupsIdT groups;
    si2drGroupIdT group;

    time_t startt, endt;

    //printf("Starting reader\n") ;
    si2drPIInit(&err);
    si2drReadLibertyFile(filename, &err);

    time(&startt);
    if( err == SI2DR_INVALID_NAME )
    {
        printf("COULD NOT OPEN %s for parsing-- quitting...\n",filename);
        return(301);
    }
    else if (err == SI2DR_SYNTAX_ERROR )
    {
        printf("\nSyntax Errors were detected in the input file!\n\n");
        return(401);
    }

    time(&endt);

    printf("... Done. \nElapsed time= %ld seconds\n", endt-startt);


    // database check
    groups = si2drPIGetGroups(&err);
    while( !si2drObjectIsNull( (group=si2drIterNextGroup(groups,&err)), &err))  {
        printf("\n\nChecking the database...\n\n");

        time(&startt);
        si2drCheckLibertyLibrary(group, &err);
        time(&endt);

        if( err == SI2DR_NO_ERROR )
            printf("Passed\n\n");
        else {
            printf("Errors detected during Liberty sanity check\n\n");
            return(501);
        }

        printf("... Done. \nElapsed time= %ld seconds\n", endt-startt);

    }
    si2drIterQuit(groups,&err);


    // now store into datastructure
    si2drGroupsIdT library_groups = si2drPIGetGroups(&err);
    si2drGroupIdT library_group;

    // iterate through library group
    while( !si2drObjectIsNull((library_group=si2drIterNextGroup(library_groups, &err)), &err) )  {

        //si2drStringT library_group_type = si2drGroupGetGroupType(library_group, &err);
        //printf(" confirm: library type = %s\n", library_group_type);

        // get the name of the library
        si2drNamesIdT library_group_names = si2drGroupGetNames(library_group, &err);
        si2drStringT library_group_name = si2drIterNextName(library_group_names, &err);
        si2drIterQuit(library_group_names, &err);
        //printf(" confirm: library name = %s\n", library_group_name);

        // create cell library
        cell_lib = new dctk::CellLib(library_group_name);


        // get the cells in the library
        si2drGroupsIdT cell_groups = si2drGroupGetGroups(library_group, &err);
        si2drGroupIdT cell_group;
        while( !si2drObjectIsNull((cell_group=si2drIterNextGroup(cell_groups, &err)), &err)) {
            si2drStringT cell_group_type =  si2drGroupGetGroupType(cell_group, &err);
            //printf(" confirm: cell type = %s\n", cell_group_type);

            // get name of cell
            si2drNamesIdT cell_group_names = si2drGroupGetNames(cell_group, &err);
            si2drStringT cell_group_name = si2drIterNextName(cell_group_names, &err);
            si2drIterQuit(cell_group_names, &err);
            //printf(" confirm: cell name = %s\n", cell_group_name);

            // if we are not looking at a "cell" group, skip
            if (strcmp(cell_group_type,"cell")) {
                continue;
            }


            // insert cell into library
            dctk::Cell* cell = new dctk::Cell(cell_group_name);
            cell_lib->insert(cell_group_name, cell);

            // get the pins in the cell
            si2drGroupsIdT pin_groups = si2drGroupGetGroups(cell_group, &err);
            si2drGroupIdT pin_group;
            while( !si2drObjectIsNull((pin_group=si2drIterNextGroup(pin_groups, &err)), &err)) {

                si2drStringT pin_group_type = si2drGroupGetGroupType(pin_group, &err);
                //printf("   confirm: pin type = %s\n", pin_group_type);

                // skip if not a "pin"
                // i.e. we don't process pg_pin, leakage_power, etc.
                if (strcmp(pin_group_type,"pin")) {
                    continue;
                }

                // get name of pin
                si2drNamesIdT pin_group_names = si2drGroupGetNames(pin_group, &err);
                si2drStringT pin_group_name = si2drIterNextName(pin_group_names, &err);
                si2drIterQuit(pin_group_names, &err);
                //printf("   confirm: pin name = %s\n", pin_group_name);

                // add pin to cell
                dctk::CellPin* pin = new dctk::CellPin(pin_group_name);
                cell->add_pin(pin_group_name, pin);

                // look for direction attribute
                si2drAttrsIdT pin_attrs = si2drGroupGetAttrs(pin_group, &err);
                si2drAttrIdT pin_attr;
                while( !si2drObjectIsNull((pin_attr=si2drIterNextAttr(pin_attrs, &err)), &err)) {
                    std::string pin_attr_name = si2drAttrGetName(pin_attr, &err);

                    // look for direction
                    if (pin_attr_name == "direction") {
                        std::string direction_str = si2drSimpleAttrGetStringValue(pin_attr, &err);
                        pin->set_direction(direction_str);
                    }
                }
                si2drIterQuit(pin_attrs, &err);

                // get the arcs attached to the pin
                si2drGroupsIdT arc_groups = si2drGroupGetGroups(pin_group, &err);
                si2drGroupIdT arc_group;
                while( !si2drObjectIsNull((arc_group=si2drIterNextGroup(arc_groups, &err)), &err)) {

                    si2drStringT arc_group_type = si2drGroupGetGroupType(arc_group, &err);
                    //printf("     confirm: arc type = %s\n", arc_group_type);

                    // get name of group (template)
                    //si2drNamesIdT arc_group_names = si2drGroupGetNames(arc_group, &err);
                    //si2drStringT arc_group_name = si2drIterNextName(arc_group_names, &err);
                    //si2drIterQuit(arc_group_names, &err);
                    //printf("     confirm: arc template name = %s\n", arc_group_name);

                    // skip if not a timing arc
                    if (strcmp(arc_group_type, "timing")) {
                        continue;
                    }

                    // define arc variable for use

                    // code to iterate through all attributes of timing group
                    std::string related_pin_str;
                    // default timing_type is "combinational"
                    std::string timing_type_str("combinational");
                    std::string timing_sense_str;

                    si2drAttrsIdT attrs = si2drGroupGetAttrs(arc_group, &err);
                    si2drAttrIdT attr;
                    while( !si2drObjectIsNull((attr=si2drIterNextAttr(attrs, &err)), &err)) {
                        std::string name = si2drAttrGetName(attr, &err);

                        // look for related_pin
                        if (name == "related_pin") {
                            related_pin_str = si2drSimpleAttrGetStringValue(attr, &err);

                        }

                        // look timing_type
                        if (name == "timing_type") {
                            timing_type_str = si2drSimpleAttrGetStringValue(attr, &err);
                        }

                        // look for timing_sense
                        if (name == "timing_sense") {
                            timing_sense_str = si2drSimpleAttrGetStringValue(attr, &err);
                        }

                    }
                    si2drIterQuit(attrs, &err);

                    // if a related pin was found, create the arc and add it to the pin
                    // note:  we will need to convert relate_pin into a pointer at some point
                    dctk::CellArc* arc = NULL;
                    if (related_pin_str.size() != 0) {
                        arc = new dctk::CellArc(pin, related_pin_str);
                        pin->add_arc(arc);
                    } else {
                        printf("ERROR: Could not find related_pin for timing arc.  Skipping this arc.");
                        break;
                    }

                    // add timing_type
                    arc->set_timing_type(timing_type_str);
                    arc->set_timing_sense(timing_sense_str);

                    // look for tables
                    si2drGroupsIdT table_groups = si2drGroupGetGroups(arc_group, &err);
                    si2drGroupIdT table_group;

                    while( !si2drObjectIsNull((table_group=si2drIterNextGroup(table_groups, &err)), &err)) {

                        si2drStringT table_group_type = si2drGroupGetGroupType(table_group, &err);
                        //printf("       confirm: table type = %s\n", table_group_type);

                        // get name of table
                        //si2drNamesIdT table_group_names = si2drGroupGetNames(table_group, &err);
                        //si2drStringT table_group_name = si2drIterNextName(table_group_names, &err);
                        //si2drIterQuit(table_group_names, &err);
                        //printf("       confirm: table name = %s\n", table_group_name);

                        // store NLDM tables
                        if (!strcmp(table_group_type,"cell_rise")) {
                            //printf("storing cell_rise\n");
                            arc->set_cell_rise_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"cell_fall")) {
                            //printf("storing cell_fall\n");
                            arc->set_cell_fall_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"rise_transition")) {
                            //printf("storing rise_transition\n");
                            arc->set_rise_transition_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"fall_transition")) {
                            //printf("storing fall_transition\n");
                            arc->set_fall_transition_table(liberty_get_values_data(table_group));
                        }

                        // store Receiver Capacitance tables
                        if (!strcmp(table_group_type,"receiver_capacitance1_fall")) {
                            //printf("storing receiver_capacitance1_fall\n");
                            arc->set_receiver_capacitance1_fall_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"receiver_capacitance1_rise")) {
                            //printf("storing receiver_capacitance1_rise\n");
                            arc->set_receiver_capacitance1_rise_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"receiver_capacitance2_fall")) {
                            //printf("storing receiver_capacitance2_fall\n");
                            arc->set_receiver_capacitance2_fall_table(liberty_get_values_data(table_group));
                        }
                        if (!strcmp(table_group_type,"receiver_capacitance2_rise")) {
                            //printf("storing receiver_capacitance2_rise\n");
                            arc->set_receiver_capacitance2_rise_table(liberty_get_values_data(table_group));
                        }


                        if (!strcmp(table_group_type,"output_current_rise")) {

                            //now collect ccs table vectors
                            si2drGroupsIdT vector_groups = si2drGroupGetGroups(table_group, &err);
                            si2drGroupIdT vector_group;

                            while( ! si2drObjectIsNull((vector_group=si2drIterNextGroup(vector_groups, &err)), &err)) {

                                si2drStringT vector_group_type = si2drGroupGetGroupType(vector_group, &err);
                                //printf("     confirm: vector type = %s\n", vector_group_type);


                                // get template used for vector
                                //si2drNamesIdT vector_group_names = si2drGroupGetNames(vector_group, &err);
                                //si2drStringT vector_group_name = si2drIterNextName(vector_group_names, &err);
                                //si2drIterQuit(vector_group_names, &err);
                                //printf("     cnfirm: vector name = %s\n", vector_group_name);

                                // store CCS table
                                if (!strcmp(vector_group_type,"vector")) {
                                    arc->add_current_rise_table(liberty_get_values_data(vector_group));
                                }

                            }
                            si2drIterQuit(vector_groups, &err);
                        }


                        if (!strcmp(table_group_type,"output_current_fall")) {

                            //now collect ccs table vectors
                            si2drGroupsIdT vector_groups = si2drGroupGetGroups(table_group, &err);
                            si2drGroupIdT vector_group;

                            while( ! si2drObjectIsNull((vector_group=si2drIterNextGroup(vector_groups, &err)), &err)) {

                                si2drStringT vector_group_type = si2drGroupGetGroupType(vector_group, &err);
                                //printf("     confirm: vector type = %s\n", vector_group_type);

                                // get template used for vector
                                //si2drNamesIdT vector_group_names = si2drGroupGetNames(vector_group, &err);
                                // si2drStringT vector_group_name = si2drIterNextName(vector_group_names, &err);
                                //si2drIterQuit(vector_group_names, &err);
                                //printf("     cnfirm: vector name = %s\n", vector_group_name);

                                // store CCS table
                                if (!strcmp(vector_group_type,"vector")) {
                                    arc->add_current_fall_table(liberty_get_values_data(vector_group));
                                }
                            }
                            si2drIterQuit(vector_groups, &err);
                        }

                    }
                    si2drIterQuit(table_groups, &err);

                }
                si2drIterQuit(arc_groups, &err);

            }
            si2drIterQuit(pin_groups, &err);

            // fix up arcs to have pointers to pins, for consistency
            cell->fixup_pins_in_arcs();


        }
        si2drIterQuit(cell_groups, &err);

    }
    si2drIterQuit(library_groups, &err);

    // dump the cell library that we just created
    // cell_lib->dump();

    //
    // Print stats
    //
    int cellcount=0;

    printf("\n\nStats:\n");
    groups = si2drPIGetGroups(&err);

    while( !si2drObjectIsNull((group=si2drIterNextGroup(groups,&err)),&err) ) {
        si2drNamesIdT gnames;
        si2drStringT gname;
        si2drGroupsIdT gs2;
        si2drGroupIdT g2;

        gnames = si2drGroupGetNames(group, &err);
        gname = si2drIterNextName(gnames,&err);
        si2drIterQuit(gnames,&err);

        if( !gname )
            gname = si2drStringT("<NONAME>");

        printf("  Library %s:\n", gname);
        gs2 = si2drGroupGetGroups(group,&err);

        while( !si2drObjectIsNull((g2=si2drIterNextGroup(gs2,&err)),&err) )  {
            si2drStringT gt = si2drGroupGetGroupType(g2,&err);

            if( !strcmp(gt,"cell") )
                cellcount++;
        }
        si2drIterQuit(gs2,&err);
        printf("  cells: %d\n\n", cellcount);
    }
    si2drIterQuit(groups,&err);

    //print_strtab_stats();
    printf("Quitting..\n");
    si2drPIQuit(&err);
    printf("Done....\n");

    return(0);

}
