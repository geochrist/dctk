* SPICE circuit for net_4
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I9:A 0 pwl(0p 0v 346.585p 0v 365.489p 0.021v 378.092p 0.07v 389.12p 0.111121v 401.723p 0.15489v 414.326p 0.195562v 426.929p 0.233459v 439.532p 0.26887v 453.711p 0.306056v 482.068p 0.373242v 497.821p 0.407071v 513.575p 0.438805v 548.234p 0.502518v 586.043p 0.564588v 630.154p 0.63v 660.086p 0.671288v 681.755p 0.7v 1528.34p 0.7v 1547.24p 0.679v 1559.85p 0.63v 1570.88p 0.588879v 1583.48p 0.54511v 1596.08p 0.504438v 1608.68p 0.466541v 1621.29p 0.43113v 1635.47p 0.393944v 1663.82p 0.326758v 1679.58p 0.292929v 1695.33p 0.261195v 1729.99p 0.197482v 1767.8p 0.135412v 1811.91p 0.07v 1841.84p 0.028712v 1863.51p 0v)
* Driver
XI9 VSS VDD I9:A I9:Y INVx13_ASAP7_75t_R
* NET
C0 I9:Y VSS 75.9FF 
C1 I10:A VSS 75.9FF 
R0 I9:Y I10:A 2179.5
* Load
XI10 VSS VDD I10:A I10:Y BUFx5_ASAP7_75t_R
* Load parasitics
Cnear I10:Y 0 105.644f
.tran 0.1p 2363.51p 0 0.1p
.measure tran fall_delay trig v(I9:A)=0.35v rise=1 targ v(I9:Y)=0.35v fall=1
.measure tran rise_delay trig v(I9:A)=0.35v fall=1 targ v(I9:Y)=0.35v rise=1
.measure tran rise_slew trig v(I9:Y)=0.07v rise=1 targ v(I9:Y)=0.63 rise=1
.measure tran fall_slew trig v(I9:Y)=0.63v fall=1 targ v(I9:Y)=0.07 fall=1
.end
