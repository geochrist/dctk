* SPICE circuit for net_3
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I7:A 0 pwl(0p 0v 294.695p 0v 310.77p 0.021v 321.486p 0.07v 330.863p 0.111121v 341.579p 0.15489v 352.295p 0.195562v 363.011p 0.233459v 373.727p 0.26887v 385.783p 0.306056v 409.895p 0.373242v 423.29p 0.407071v 436.685p 0.438805v 466.155p 0.502518v 498.303p 0.564588v 535.81p 0.63v 561.261p 0.671288v 579.686p 0.7v 1374.38p 0.7v 1390.46p 0.679v 1401.17p 0.63v 1410.55p 0.588879v 1421.26p 0.54511v 1431.98p 0.504438v 1442.7p 0.466541v 1453.41p 0.43113v 1465.47p 0.393944v 1489.58p 0.326758v 1502.98p 0.292929v 1516.37p 0.261195v 1545.84p 0.197482v 1577.99p 0.135412v 1615.5p 0.07v 1640.95p 0.028712v 1659.37p 0v)
* Driver
XI7 VSS VDD I7:A I7:Y HB4xp67_ASAP7_75t_R
* NET
C0 I7:Y VSS 7.1875FF 
C1 I8:A VSS 7.1875FF 
R0 I7:Y I8:A 293.875
* Load
XI8 VSS VDD I8:A I8:Y BUFx8_ASAP7_75t_R
* Load parasitics
Cnear I8:Y 0 316.427f
.tran 0.1p 2159.37p 0 0.1p
.measure tran rise_delay trig v(I7:A)=0.35v rise=1 targ v(I7:Y)=0.35 rise=1
.measure tran fall_delay trig v(I7:A)=0.35v fall=1 targ v(I7:Y)=0.35v fall=1
.measure tran rise_slew trig v(I7:Y)=0.07v rise=1 targ v(I7:Y)=0.63 rise=1
.measure tran fall_slew trig v(I7:Y)=0.63v fall=1 targ v(I7:Y)=0.07 fall=1
.end
