Installation Instructions
=========================

Overview
--------

1.  Install Parser-SPEF in a directory parallel to this one.
2.  Install yaml-cpp in a directory parallel to this one.
3.  Build src-liberty_parse-2.6.
4.  Download ASU 7nm Predictive Libraries.
5.  Download Nangate FreePDK45 Library (for cells) (optional -- not needed for contest)
6.  Download OpenRoad Utilities (for SPICE models) (optional -- not needed for contest)
7.  Download NCSU FreePDK45 version 1.4 (optional -- not needed for contest)
8.  Download and install ngspice (optional -- not needed for contest)
9.  Download and install Xyce.
10. Compile and test dctk.

Details
-------

For the text below, assume:

    $DIR = the directory of dctk download.

### Parser-SPEF

  Download Parser-SPEF in a directory parallel this one:

    cd $DIR/..

    git clone https://github.com/OpenTimer/Parser-SPEF.git

  The sub-directory Parser-SPEF will appear.  No compilation is
  necessary -- all files are header files to be included by the
  calling code.

### yaml-cpp

  Download yaml-cpp in a directory parallel this one:

    cd $DIR/..
    git clone https://github.com/jbeder/yaml-cpp.git

  To build it using standard cmake build process:

    cd $DIR/../cpp-yaml
    mkdir build
    cd build
    cmake ..
    make

### src-liberty_parse-2.6

  First set the CFLAGS to skip no-return-type errors.  It is
  needed for most modern versions of compilers:

    setenv CFLAGS -Wno-return-type

  Then go through the standard configure build process:

    cd $DIR/src-liberty_parse-2.6
    ./configure
    make

  (If the above 'make' command does not complete,
  please try 'make clean' before running 'make')

  Finally, dctk requires static libraries.  Run the custom library
  archiving script in the directory:

    makelib

  The result should be a file named liblibparse.a in the src-liberty_parse-2.6
  directory.

### ASU 7nm Predictive Libraries

  ***The current plan of record is use these libraries for the competition.***

  Request access to the ASU predictive 7nm libraries through the form at
  http://asap.asu.edu/asap. (Scroll to the bottom to the Donwload button and click
  through the agreement pages. Then fill in your details to get a download link.)

  Once you get the download, unzip/untar it at the following directory:

    cd $DIR/..

  You should have a subdirectory directory named `ASAP7_PDKandLIB_v1p6`.

  You'll need to postprocess the library for it to work with the parsers.  Do the following

    cd ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/LIB/CCS
    sed '/waveform_time_template/,/\}/d ; /^[[:space:]]*driver_waveform/d' asap7sc7p5t_INVBUF_RVT_TT_ccs_191031.lib > asap7sc7p5t_INVBUF_RVT_TT_ccs_191031.postprocessed.lib

  You'll need to make a final manual edit to add the following information into the library scope:

    voltage_map(VDD, 0.7);
    voltage_map(VSS, 0.0);
  
  Note that we are postprocessing only the inverter and buffer library since we are limiting the TAU 2020 Contest to inverters and buffers.

  Other information

  * spice netlists are available at `ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp`
  * spice models are available at `ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm`

The spice netlists are not quite in the right format.  The "W=<width>" parameter needs to be removed as well as all parameters prefixed with \$.  Use the following commands in the directory:
  
    sed 's/ W=[a-zA-Z0-9\.\+-]*//g; s/\$.*//g' asap7sc7p5t_INVBUF_RVT.sp > asap7sc7p5t_INVBUF_RVT.sp.modified

hereafter, use the *.modifed version of the files for SPICE simulations (below).

In addtion, note that the spice models are level 72 (bsimcmg/finfets) and need Spectre for simulation, as-is, but to get these models to work with Xyce, please change the level value to 107.
(Name the modified model files with a .modified suffix.)


### Nangate FreePDK45 Library (does not include spice models) (optional, not needed for contest)

  While we currently are not ***not*** planning to use these libraries for the competition, these can be useful for development because they are
  built on spice models supported by ngspice.
  
  Download the `NangateOpenCellLibrary_PDKv1_3_v2010_12.tgz` from
  `https://projects.si2.org/openeda.si2.org/project/showfiles.php?group_id=63#p78`
  (You will need to register with the organization.)

  Extract in this directory:  

    cd $DIR/..
    tar xvf NangateOpenCellLibrary_PDKv1_3_v2010_12.tgz

  You'll then get a subdirectory named
  `NangateOpenCellLibrary_PDKv1_3_v2010_12/`.  The CCS Liberty models
  are in the Front_End/Liberty/CCS directory.  They will need to be
  pre-processed to work with the liberty_parser installed earlier.
  Please see dctk/etc/preprocess_nangate/ for scripts.

  Since the contest will be focusing on only inverters and buffers,
  remove all cells except inverters and buffers.  (A copy of a
  resulting file is in `$DIR/test/buf_inv.lib `and used by `$DIR/etc/runme.nangate`.)

  The SPICE models possibly corresponding to the Library are
  available from the OpenROAD-Utilities repository (see below).

### OpenROAD-Utilities (optional -- not needed for contest)

  To download the OpenRoad Utilities:

    cd $DIR/..
    git clone https://github.com/The-OpenROAD-Project/OpenROAD-Utilities.git

  The HSPICE models that somewhat correlate to the Nangate FreePDK45 Library are in
 ` OpenROAD-Utilities/TimerCalibration/Free45PDK/gpdk45nm.m`.

  The model names don't quite match -- they need to be edited.  Change the model names from

      nmos ==> nmos_vtl
      pmos ==> pmos_vtl

  in a new file named `OpenROAD-Utilities/TimerCalibration/Free45PDK/gpdk45nm.m.modified`
  These models will work with ngspice.

  Note that we are not sure that these models match the ones used to
  generate the Nangate Open Cell Library and should be used
  temporarily until Liberty models with corresponding SPICE models are
  available.  In addition, it seems that only models for the nominal
  corner is available.

### NCSU FreePDK45 version 1.4 (optional -- not needed for contest)

  The Nangate library is based on the NCSU FreePDK45 models.  This seems to no longer be
  available at NCSU, but an archived version (1.4) seems to be available at `https://gitlab.esat.kuleuven.be/Thomas.Vandenabeele/digital-design-flow/blob/6b5823be96ec7b947dfad95c576499e830465ed8/99_SRC/technology/NCSU-FreePDK45-1.4/ncsu-FreePDK45-1.4.tar.gz`

  Download the file to this location and extract using:

    cd $DIR/..
    gzcat ncsa-FreePDK45-1.4.tar.gz | tar xvf -
  
  The models will be at `FreePDK45/ncsu_basekit/models/hspice/*.inc`.

  But the Nangate library indicated that it was characterized using
  version 1.3, and the release notes for FreePDK45 version 1.4
  indicated that changes were made in the HSPICE models.  Hence, we
  are not certain that the Nangate library would correspond well with
  these spice models.  These have not been tested.

### Download and install Xyce

  Go to xyce.sandia.gov for more information

### Download and install ngspice (optional -- not needed for contest)

  Go to ngspice.sourceforge.net for more information

### Compile and test dctk

  Go through the standard process for cmake:

    cd $DIR
    mkdir build
    cd build
    cmake ..

  The result should be build/delay_calc_tool executable

  Test this executable:

    cd $DIR/test
    ./runme.asu     # for ASU -- requires Xyce
 

  The result should be:
  
  * random circuits based on inverters/buffers only (tau2020_asu.circuits.yaml)
  * corresponding spice decks (spice_decks/)
  * automatically run Xyce

  The SPICE decks for the ASU-based test circuits should work for Xyce -- this has been tested.

 

  




