Installation Instructions
=========================

Overview
--------

1.  Install Parser-SPEF in a directory parallel to this one.

2.  Install yaml-cpp in a directory parallel to this one.

3.  Build src-liberty_parse-2.6.

4.  Compile dctk.

Details
-------

Assume:

  $DIR = the directory of dctk download.

### Parser-SPEF

  Download Parser-SPEF in a directory parallel this one:

    cd $DIR/..

    git clone https://github.com/OpenTimer/Parser-SPEF.git

  The sub-directory Parser-SPEF will appear.

  No compilation is necessary -- all files are header files to be
  included by the calling code.

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

  Finally, dctk requires static libraries.  Run the custom library
  archiving script in the directory:

    makelib

  The result should be a file named liblibparse.a in the src-liberty_parse-2.6
  directory.

### Nangate FreePDK45 Library (no spice models)

  cd $DIR/..

  Download the NangateOpenCellLibrary_PDKv1_3_v2010_12.tgz from
  https://projects.si2.org/openeda.si2.org/project/showfiles.php?group_id=63#p78
  (You will need to register with the organization.)

  Extract in this directory.  You'll then get a subdirectory named:

    NangateOpenCellLibrary_PDKv1_3_v2010_12/

  The CCS Liberty models are in the Front_End/Liberty/CCS directory.
  They will need to be pre-processed to work with the liberty_parser
  installed earlier.  Please see dctk/etc/preprocess_nangate/ for
  scripts.

  The HSPICE models corresponding to the Library are available from the OpenROAD-Utilities repo.

### OpenROAD-Utilities

  cd $DIR/..

  git clone https://github.com/The-OpenROAD-Project/OpenROAD-Utilities.git

  The HSPICE models that somewhat correlate to the Nangate FreePDK45 Library are in
  OpenROAD-Utilities/TimerCalibration/Free45PDK/gpdk45nm.m.

  The transistor names don't quite match ... they need to be edited.  Change the model names from

      nmos ==> nmos_vtl
      pmos ==> pmos_vtl

  in a new file named OpenROAD-Utilities/TimerCalibration/Free45PDK/gpdk45nm.m.modified

  Note that we are not 100% sure that these models match and should be used temporarily until correlated Liberty
  with SPICE models are generated.  In addition, it seems that only models for the nominal corner is available.

### NCSU FreePDK45

  The Nangate library is based on the NCSU FreePDK45 models.  This seems to no longer be
  available at NCSU, but an archived version (1.4) seems to be available at:

    https://gitlab.esat.kuleuven.be/Thomas.Vandenabeele/digital-design-flow/blob/6b5823be96ec7b947dfad95c576499e830465ed8/99_SRC/technology/NCSU-FreePDK45-1.4/ncsu-FreePDK45-1.4.tar.gz

    cd $DIR/..

  Download the file to this location and extract using:

    gzcap ncsa-FreePDK45-1.4.tar.gz | tar xvf -
  
  The models will be at FreePDK45/ncsu_basekit/models/hspice/*.inc.

  But the Nangate library indicated that it was characterized using
  version 1.3, and the release notes for FreePDK45 version 1.4
  indicated that changes were made in the HSPICE models.  Hence, we
  are not certain that the Nangate library would correspond well with
  these spice models.  These have not been tested.

### dctk

  Go through the standard process for cmake:

    cd $DIR
    mkdir build
    cd build
    cmake ..

  The result should be build/delay_calc_tool executable

  Test this executable:

    cd $DIR/test
    ./runme

  The result should be a
  * random circuits based on inverters/buffers only (tau2020v1.circuits.yaml)
  * corresponding spice decks (spice_decks/)





