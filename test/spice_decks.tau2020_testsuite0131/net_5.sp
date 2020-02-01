* SPICE circuit for net_5
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I11:A 0 pwl(0p 0v 276.599p 0v 291.686p 0.021v 301.744p 0.07v 310.545p 0.111121v 320.603p 0.15489v 330.661p 0.195562v 340.72p 0.233459v 350.778p 0.26887v 362.093p 0.306056v 384.724p 0.373242v 397.297p 0.407071v 409.869p 0.438805v 437.529p 0.502518v 467.703p 0.564588v 502.907p 0.63v 526.795p 0.671288v 544.089p 0.7v 1320.69p 0.7v 1335.78p 0.679v 1345.83p 0.63v 1354.63p 0.588879v 1364.69p 0.54511v 1374.75p 0.504438v 1384.81p 0.466541v 1394.87p 0.43113v 1406.18p 0.393944v 1428.81p 0.326758v 1441.39p 0.292929v 1453.96p 0.261195v 1481.62p 0.197482v 1511.79p 0.135412v 1547p 0.07v 1570.88p 0.028712v 1588.18p 0v)
* Driver
XI11 VSS VDD I11:A I11:Y HB4xp67_ASAP7_75t_R
* NET
C0 I11:Y VSS 5.13125FF 
C1 I12:A VSS 5.13125FF 
R0 I11:Y I12:A 406.281
* Load
XI12 I12:A VSS VDD I12:Y BUFx12f_ASAP7_75t_R
* Load parasitics
Cnear I12:Y 0 154.689f
.tran 0.1p 2088.18p 0 0.1p
.measure tran rise_delay trig v(I11:A)=0.35v rise=1 targ v(I11:Y)=0.35 rise=1
.measure tran fall_delay trig v(I11:A)=0.35v fall=1 targ v(I11:Y)=0.35v fall=1
.measure tran rise_slew trig v(I11:Y)=0.07v rise=1 targ v(I11:Y)=0.63 rise=1
.measure tran fall_slew trig v(I11:Y)=0.63v fall=1 targ v(I11:Y)=0.07 fall=1
.end
