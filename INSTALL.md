Installation Instructions
=========================

Overview
--------

1.  Install Parser-SPEF in a directory parallel to this one.
2.  Install yaml-cpp in a directory parallel to this one.
3.  Build src-liberty_parse-2.6.
4.  Download ASU 7nm Predictive Libraries. (Changed from 2020)
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

  cd $DIR/..
  git clone https://github.com/The-OpenROAD-Project/asap7.git

  You should have a subdirectory directory named asap7.

  We will be using the Liberty library

      asap7/asap7sc7p5t_27/LIB/CCS/asap7sc7p5t_INVBUF_RVT_TT_ccs_201020.lib.

  The following modifications should be made:

  1.  Replace all values of 1e-22 with 1.0000e-5

  2.  Define the following at line 84, near the other "defines"
      define(driver_waveform_fall, pin, string);
      define(driver_waveform_rise, pin, string);

  We will be using the extracted netlists:

    asap7/asap7sc7p5t_27/CDL/xAct3D_extracted/asap7sc7p5t_27_R.sp

  They need to be modified as in the TAU2020 Contest:

    The "W=<width>" parameter needs to be removed as well as all parameters prefixed with \$.  Use the following commands in the directory:
  
    sed 's/ W=[a-zA-Z0-9\.\+-]*//g; s/\$.*//g' asap7sc7p5t_27_R.sp > asap7sc7p5t_27_R.sp.modified

  We will be using the hspice models:

    asap7/asap7PDK_r1p7/models/hspice/hspice/7nm_TT.pm

  As before, they need to be modified to change the simulation model from 72 to 107, so as to work with Xyce.

    cd ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/LIB/CCS
    sed '/waveform_time_template/,/\}/d ; /^[[:space:]]*driver_waveform/d' asap7sc7p5t_INVBUF_RVT_TT_ccs_191031.lib > asap7sc7p5t_INVBUF_RVT_TT_ccs_191031.postprocessed.lib


  Now, one more thing needs to be done in order to get the ASAP7nm libraries to work:  
    The ASAP7 Liberty model can't quite be read by the open source Liberty
    parser because it flags errors with less than 4 digits precision.  One
    can either fix all the numbers in the Liberty library to have 4 digits
    of precision, or just disable the error.  Here's how to disable the
    error:

  *  Edit $DIR/src-liberty_parse-2.6/syntax_checks.c:
        Comment out the following line (around line 5215):

                                        //                                      (*MsgPrinter)(SI2DR_SEVERITY_ERR, SI2DR_SEMANTIC_ERROR,                                                                     
                                        //                                                                EB,                                                                                               
                                        //                                                                &err);
    The error message will still be displayed, but it will not stop

  * Recompile (./configure && make)


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

 

  




