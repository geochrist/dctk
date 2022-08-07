Installation Instructions
=========================

Overview
--------

1.  Install OpenTimer in a directory parallel to this one.
2.  Install yaml-cpp in a directory parallel to this one.
3.  Build src-liberty_parse-2.6.
4. Compile and test dctk.

Details
-------

For the text below, assume:

    $DIR = the directory of dctk download.

### OpenTimer

  Download OpenTimer in a directory parallel this one:

    cd $DIR/..

    git clone https://github.com/OpenTimer/OpenTimer.git

  Multiple subdirectories will appear.

  Follow instructions in the README.md to compile.
  (It is required to compile for dctk to be able to access the code base.)

### yaml-cpp

  Download yaml-cpp in a directory parallel this one:

    cd $DIR/..
    git clone https://github.com/jbeder/yaml-cpp.git

  To build it using standard cmake build process:

    cd $DIR/../yaml-cpp
    mkdir build
    cd build
    cmake ..
    setenv CFLAGS -Wno-shadow
    make


### src-liberty_parse-2.6

  First set the CFLAGS to skip no-return-type errors.  It is
  needed for most modern versions of compilers:

    setenv CFLAGS -Wno-return-type

  Then go through the standard configure build process:

    cd $DIR/src-liberty_parse-2.6
    # see notes below if installing on recent Mac version
    ./configure
    make

  **
  ** Notes for Mac OSX
  ** Latest gcc that comes with Mac OSX does not work.
  ** Install a different compiler (e.g. homebrew gcc-11) and issue
  **   setenv CC /opt/homebrew/bin/gcc-11 
  ** After compilation, restore original environment
  **

  (If the above 'make' command does not complete,
  please try 'make clean' before running 'make')

  Finally, dctk requires static libraries.  Run the custom library
  archiving script in the directory:

    makelib

  The result should be a file named liblibparse.a in the src-liberty_parse-2.6
  directory.

### Compile and test dctk

  Go through the standard process for cmake:

    cd $DIR
    mkdir build
    cd build
    cmake ..
    make

  The result should be build/delay_calc_tool executable

  Test this executable:

    cd $DIR/test
    ./runme.ptg     

  The result should be:
  
     TBD
     
 

  




