* SPICE circuit for net_1
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I3:A 0 pwl(0p 0v 201.671p 0v 212.671p 0.021v 220.004p 0.07v 226.421p 0.111121v 233.755p 0.15489v 241.088p 0.195562v 248.422p 0.233459v 255.755p 0.26887v 264.005p 0.306056v 280.506p 0.373242v 289.672p 0.407071v 298.839p 0.438805v 319.006p 0.502518v 341.007p 0.564588v 366.674p 0.63v 384.091p 0.671288v 396.7p 0.7v 1098.37p 0.7v 1109.37p 0.679v 1116.7p 0.63v 1123.12p 0.588879v 1130.45p 0.54511v 1137.79p 0.504438v 1145.12p 0.466541v 1152.45p 0.43113v 1160.71p 0.393944v 1177.21p 0.326758v 1186.37p 0.292929v 1195.54p 0.261195v 1215.71p 0.197482v 1237.71p 0.135412v 1263.37p 0.07v 1280.79p 0.028712v 1293.4p 0v)
* Driver
XI3 VSS VDD I3:A I3:Y INVx4_ASAP7_75t_R
* NET
C0 I3:Y VSS 51.875FF 
C1 I4:A VSS 51.875FF 
R0 I3:Y I4:A 311.75
* Load
XI4 VSS VDD I4:A I4:Y BUFx6f_ASAP7_75t_R
* Load parasitics
Cnear I4:Y 0 149.978f
.tran 0.1p 1793.4p 0 0.1p
.measure tran fall_delay trig v(I3:A)=0.35v rise=1 targ v(I3:Y)=0.35v fall=1
.measure tran rise_delay trig v(I3:A)=0.35v fall=1 targ v(I3:Y)=0.35v rise=1
.measure tran rise_slew trig v(I3:Y)=0.07v rise=1 targ v(I3:Y)=0.63 rise=1
.measure tran fall_slew trig v(I3:Y)=0.63v fall=1 targ v(I3:Y)=0.07 fall=1
.end
