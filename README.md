# Delay Calculation ToolKit (dctk)

## Introduction

This toolkit is currently used to house the files for the TAU 2020 contest.
Please look in INSTALL.md for installation and testing directions.

## Inventory
`src/` contains the source files for the infrastructure. The contestants
should not touch these files (but report any bugs found).

`src-dut/` is expected to contain the contestants source files or
linkable library.  Currently, in this directory, is an example call
which randomly generates a delay and slew for a "circuit".  (Result is
obviously wrong.)  Look at the routine compute\_delays() in
src-dut/delay\_calculator.cpp and its call in src/delay_calc_tool.cpp
to see what the API is expected to look like.

`test/` shows how random circuits will be generated, for testing
purposes. The source code for gen\_random_nets is in src.  Contestants
can use this code to generate their own testsuites.

## Programs and Scripts

### test/runme.asu

This csh script demonstrates all steps of the benchmarking process with
the ASU library:

* generate a testsuite with 10 random circuits
* generate SPICE decks for the testsuite
* run Xyce (sequentially) on the testsuite to produce golden results
* run contestant code on the testsuite
* merge and score the contestants results with the golden results

Note that the user needs to have installed the ASU libraries
according to the installation instructions.

### test/runme.nangate

This csh script demonstrates all steps of the benchmarking process with
the Nangate library:

* generate a testsuite with 10 random circuits
* generate SPICE decks for the testsuite
* run Xyce (sequentially) on the testsuite to produce golden results
* run contestant code on the testsuite
* merge and score the contestants results with the golden results

Note that the user needs to have installed the Nangate libraries
according to the installation instructions.

(This script is not used for the TAU 2020 contest, but it was provided for early development while the ASU library was being brought up.)

### test/run_testsuite TESTSUITE

This csh script is similar to runme.asu except it only runs the contestants code and merges and scores the results.  It does not generate the testsuite.

### test/runme.gen_testsuite

This csh script describes how the testsuite will be generated.  It will generate a set of random circuits before running simulations (in parallel via dispatch_parallel_sims) to generate golden results.

ASU library will be used.

### etc/dispatch_parallel_sims

This Python scripts dispatches Xyce simulation runs in parallel.

```
dispatch_parallel_sims options

Options:
  --spice_decks DIRECTORY         directory with SPICE decks (required)
  --num_cores NUM                 number of cores to use
 
```
The results of the simulation will stay in the DIRECTORY specified.  

### build/gen\_random_nets

This application generates random circuits based on the Liberty models.

```
gen_random_nets options

options:
   --liberty LIBERTY_FILE     Liberty models to consider for driver and receivers (required)
   --dataset TESTSUITE        name of testsuite.  Results will be in $testsuite.circuits.yaml $testsuite.spef (required)
   -n NUM                     number of circuits (required)
   --pimodels                 force interconnect to a pi model format (optional)
   --waveform WAVEFORM        either ramp or asu_exp (required)
```
For the TAU 2020 Contest, we will always set `--pimodels` and `--waveform asu_exp`.


### build/delay\_calc_tool

This application is the heart of the toolkit.  It runs in the following modes:

* generate spice decks
* run simulations (Xyce or ngspice)
* run delay calculation code
* merge and score golden results

##### generate spice decks

```
delay_calc_tool --gen_spice_decks options

options:
   --spice_dir DIRECTORY      directory for spice netlists (required)
   --circuits CIRCUITS        *.circuits.yaml (required)
   --spef SPEF                *.spef corresponding to the *.circuits.yaml (required)
   --liberty LIBERTY_FILE     Liberty Models for cells (required)
   --spice_lib SPICE_MODEL    SPICE models (required)
   --simulator SIMULATOR      simulator (xyce or ngspice) (required) 
   --run_sims                 run simulations (optional)

```

Note that SPICE decks generated are simulator-specific.  (MEASURE syntax are different between xyce and ngspice.)


##### run simulations

```
delay_calc_tool --run_sims options

options:
   --circuits CIRCUITS.       *.circuits.yaml (required)
   --spice_dir DIRECTORY      directory for spice netlists (required)
   --simulator SIMULATOR      simulator (xyce or ngspice) (required) 

```

This type of invocation does not regenerate the spice decks.

##### run delay calculation
```
delay_calc_tool --dc_file DC_OUTPUT options

options:
   --circuits CIRCUITS        *.circuits.yaml (required)
   --spef SPEF                *.spef corresponding to the *.circuits.yaml (required)
   --liberty LIBERTY_FILE     Liberty Models for cells (required)

```

`DC_OUTPUT` is the output *.circuits.yaml file that will contain the delay calculation output.

##### merge and score
```
delay_calc_tool --merge_circuits MERGE_OUTPUT options

options:
   --circuits CIRCUITS        *.circuits.yaml (required)
   --spice_dir SPICE_DIR      directory with SPICE results (required)
   --simulator SIMULATOR      simulator (xyce or ngspice) (required) 
```

`MERGE_OUTPUT` is the *.circuits.yaml file that includes golden, delay calculation, and scoring results.

## Conventions

* $testsuite.circuits.yaml -- Circuit files containing only the testsuite
* $testsuite.dc.circuits.yaml -- Circuit files containing delay calculation results
* $testsuite.merge.circuits.yaml -- Circuit files containing both golden and delay calculation results (and scoring)

## Future
* re-work interface to be cleaner, and more consistent.
* add support for multi-input cells (sensitization of for side input pins).


## Available tags

tau2020_alpha200119 -- build against this tag for alpha release
tau2020_alpha200120 -- OK to build against this tag for alpha release
tau2020_alpha200126
tau2020_alpha200202
tau2020_alpha200205
tau2020_alpha200206
tau2020_beta200208 -- version to use for pre-submissions





