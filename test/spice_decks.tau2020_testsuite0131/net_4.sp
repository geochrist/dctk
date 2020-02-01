* SPICE circuit for net_4
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I9:A 0 pwl(0p 0v 237.376p 0v 250.323p 0.021v 258.955p 0.07v 266.508p 0.111121v 275.14p 0.15489v 283.772p 0.195562v 292.404p 0.233459v 301.035p 0.26887v 310.746p 0.306056v 330.168p 0.373242v 340.958p 0.407071v 351.748p 0.438805v 375.485p 0.502518v 401.38p 0.564588v 431.592p 0.63v 452.093p 0.671288v 466.934p 0.7v 1204.31p 0.7v 1217.26p 0.679v 1225.89p 0.63v 1233.44p 0.588879v 1242.07p 0.54511v 1250.71p 0.504438v 1259.34p 0.466541v 1267.97p 0.43113v 1277.68p 0.393944v 1297.1p 0.326758v 1307.89p 0.292929v 1318.68p 0.261195v 1342.42p 0.197482v 1368.31p 0.135412v 1398.53p 0.07v 1419.03p 0.028712v 1433.87p 0v)
* Driver
XI9 VSS VDD I9:A I9:Y INVx2_ASAP7_75t_R
* NET
C0 I9:Y VSS 40.0625FF 
C1 I10:A VSS 40.0625FF 
R0 I9:Y I10:A 308.025
* Load
XI10 VSS VDD I10:A I10:Y INVx3_ASAP7_75t_R
* Load parasitics
Cnear I10:Y 0 39.0191f
.tran 0.1p 1933.87p 0 0.1p
.measure tran fall_delay trig v(I9:A)=0.35v rise=1 targ v(I9:Y)=0.35v fall=1
.measure tran rise_delay trig v(I9:A)=0.35v fall=1 targ v(I9:Y)=0.35v rise=1
.measure tran rise_slew trig v(I9:Y)=0.07v rise=1 targ v(I9:Y)=0.63 rise=1
.measure tran fall_slew trig v(I9:Y)=0.63v fall=1 targ v(I9:Y)=0.07 fall=1
.end
