* SPICE circuit for net_9
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I19:A 0 pwl(0p 0v 335.369p 0v 353.661p 0.021v 365.857p 0.07v 376.527p 0.111121v 388.723p 0.15489v 400.918p 0.195562v 413.113p 0.233459v 425.308p 0.26887v 439.028p 0.306056v 466.467p 0.373242v 481.711p 0.407071v 496.955p 0.438805v 530.492p 0.502518v 567.077p 0.564588v 609.761p 0.63v 638.725p 0.671288v 659.693p 0.7v 1495.06p 0.7v 1513.35p 0.679v 1525.55p 0.63v 1536.22p 0.588879v 1548.42p 0.54511v 1560.61p 0.504438v 1572.81p 0.466541v 1585p 0.43113v 1598.72p 0.393944v 1626.16p 0.326758v 1641.4p 0.292929v 1656.65p 0.261195v 1690.18p 0.197482v 1726.77p 0.135412v 1769.45p 0.07v 1798.42p 0.028712v 1819.39p 0v)
* Driver
XI19 VSS VDD I19:A I19:Y INVx3_ASAP7_75t_R
* NET
C0 I19:Y VSS 24.75FF 
C1 I20:A VSS 24.75FF 
R0 I19:Y I20:A 537.5
* Load
XI20 VSS VDD I20:A I20:Y INVx5_ASAP7_75t_R
* Load parasitics
Cnear I20:Y 0 117.418f
.tran 0.1p 2319.39p 0 0.1p
.measure tran fall_delay trig v(I19:A)=0.35v rise=1 targ v(I19:Y)=0.35v fall=1
.measure tran rise_delay trig v(I19:A)=0.35v fall=1 targ v(I19:Y)=0.35v rise=1
.measure tran rise_slew trig v(I19:Y)=0.07v rise=1 targ v(I19:Y)=0.63 rise=1
.measure tran fall_slew trig v(I19:Y)=0.63v fall=1 targ v(I19:Y)=0.07 fall=1
.end
