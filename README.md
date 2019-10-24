# dctk
Delay Calculation ToolKit

This toolkit is currently used to house the files for the TAU 2020 contest.

Please look in INSTALL.txt for installation and testing directions.

src/ contains the source files for the infrastructure. The contestants
should not touch these files (but report any bugs found).

src-dut/ is expected to contain the contestants source files or
linkable library.  Currently, in this directory, is an example call
which randomly generates a delay and slew for a "circuit".  (Result is
obviously wrong.)  Look at the routine compute_delays() in
src-dut/delay_calculator.cpp and its call in src/delay_calc_tool.cpp
to see what the API is expected to look like.

test/ shows how random circuits will be generated, for testing
purposes. The source code is in src/. Contestants can use this code to
generate their own testcases.


** Note that this code is still under development and APIs may change,
   but it shouldn't be too significant at this point. **


TODO

* add a spef reader to delay_calc_tool
* add a SPICE deck writer to delay_calc_tool
* add call to spice simulator into delay_calc_tool
* add comparator to delay_calc_tool
* add net reduction code to gen_random_nets
