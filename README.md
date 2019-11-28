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
obviously wrong.)  Look at the routine compute_delays() in
src-dut/delay_calculator.cpp and its call in src/delay_calc_tool.cpp
to see what the API is expected to look like.

`test/` shows how random circuits will be generated, for testing
purposes. The source code gen_random_nets.cpp is in src.  Contestants
can use this code to generate their own testcases.

**Note that this code is still under development and APIs may change,
   but it shouldn't be too significant at this point.**

## SPICE Deck Writer

The spice deck writer is functional for single input/output cells.
It needs updates to handle multi input/output cells.

## Todo

* find open source spice simulator supporting Level 72 models
* add call to spice simulator into delay_calc_tool.
* add support for multi-input cells (sensitization of for side input pins).
* add comparator to delay_calc_tool.
