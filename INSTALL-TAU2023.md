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

  No need to compile, but you may want to test your download.
  Follow instructions in the README.md. 

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

### Compile and test dctk

  Go through the standard process for cmake:

    cd $DIR
    mkdir build
    cd build
    cmake ..

  The result should be build/delay_calc_tool executable

  Test this executable:

    cd $DIR/test
    ./runme.ptg     

  The result should be:
  
     TBD
     
 

  




