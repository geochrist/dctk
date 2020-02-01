* SPICE circuit for net_6
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I13:A 0 pwl(0p 0v 297.279p 0v 313.495p 0.021v 324.305p 0.07v 333.764p 0.111121v 344.574p 0.15489v 355.384p 0.195562v 366.194p 0.233459v 377.004p 0.26887v 389.166p 0.306056v 413.489p 0.373242v 427.001p 0.407071v 440.514p 0.438805v 470.242p 0.502518v 502.672p 0.564588v 540.508p 0.63v 566.182p 0.671288v 584.769p 0.7v 1382.05p 0.7v 1398.26p 0.679v 1409.07p 0.63v 1418.53p 0.588879v 1429.34p 0.54511v 1440.15p 0.504438v 1450.96p 0.466541v 1461.77p 0.43113v 1473.93p 0.393944v 1498.26p 0.326758v 1511.77p 0.292929v 1525.28p 0.261195v 1555.01p 0.197482v 1587.44p 0.135412v 1625.28p 0.07v 1650.95p 0.028712v 1669.54p 0v)
* Driver
XI13 VSS VDD I13:A I13:Y INVx3_ASAP7_75t_R
* NET
C0 I13:Y VSS 6.7FF 
C1 I14:A VSS 6.7FF 
R0 I13:Y I14:A 357
* Load
XI14 VSS VDD I14:A I14:Y BUFx8_ASAP7_75t_R
* Load parasitics
Cnear I14:Y 0 33.1242f
.tran 0.1p 2169.54p 0 0.1p
.measure tran fall_delay trig v(I13:A)=0.35v rise=1 targ v(I13:Y)=0.35v fall=1
.measure tran rise_delay trig v(I13:A)=0.35v fall=1 targ v(I13:Y)=0.35v rise=1
.measure tran rise_slew trig v(I13:Y)=0.07v rise=1 targ v(I13:Y)=0.63 rise=1
.measure tran fall_slew trig v(I13:Y)=0.63v fall=1 targ v(I13:Y)=0.07 fall=1
.end
