* SPICE circuit for net_3
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I7:A 0 pwl(0p 0v 357.84p 0v 377.359p 0.021v 390.371p 0.07v 401.757p 0.111121v 414.769p 0.15489v 427.782p 0.195562v 440.794p 0.233459v 453.807p 0.26887v 468.445p 0.306056v 497.723p 0.373242v 513.989p 0.407071v 530.254p 0.438805v 566.038p 0.502518v 605.075p 0.564588v 650.619p 0.63v 681.523p 0.671288v 703.896p 0.7v 1561.74p 0.7v 1581.26p 0.679v 1594.27p 0.63v 1605.65p 0.588879v 1618.67p 0.54511v 1631.68p 0.504438v 1644.69p 0.466541v 1657.7p 0.43113v 1672.34p 0.393944v 1701.62p 0.326758v 1717.88p 0.292929v 1734.15p 0.261195v 1769.93p 0.197482v 1808.97p 0.135412v 1854.51p 0.07v 1885.42p 0.028712v 1907.79p 0v)
* Driver
XI7 VSS VDD I7:A I7:Y BUFx12_ASAP7_75t_R
* NET
C0 I7:Y VSS 87.7FF 
C1 I8:A VSS 87.7FF 
R0 I7:Y I8:A 9020
* Load
XI8 VSS VDD I8:A I8:Y INVx2_ASAP7_75t_R
* Load parasitics
Cnear I8:Y 0 87.2447f
.tran 0.1p 2407.79p 0 0.1p
.measure tran rise_delay trig v(I7:A)=0.35v rise=1 targ v(I7:Y)=0.35 rise=1
.measure tran fall_delay trig v(I7:A)=0.35v fall=1 targ v(I7:Y)=0.35v fall=1
.measure tran rise_slew trig v(I7:Y)=0.07v rise=1 targ v(I7:Y)=0.63 rise=1
.measure tran fall_slew trig v(I7:Y)=0.63v fall=1 targ v(I7:Y)=0.07 fall=1
.end
