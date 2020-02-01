* SPICE circuit for net_9
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I19:A 0 pwl(0p 0v 259.482p 0v 273.636p 0.021v 283.071p 0.07v 291.328p 0.111121v 300.763p 0.15489v 310.199p 0.195562v 319.635p 0.233459v 329.07p 0.26887v 339.685p 0.306056v 360.916p 0.373242v 372.71p 0.407071v 384.505p 0.438805v 410.453p 0.502518v 438.76p 0.564588v 471.785p 0.63v 494.195p 0.671288v 510.419p 0.7v 1269.9p 0.7v 1284.05p 0.679v 1293.49p 0.63v 1301.75p 0.588879v 1311.18p 0.54511v 1320.62p 0.504438v 1330.05p 0.466541v 1339.49p 0.43113v 1350.1p 0.393944v 1371.33p 0.326758v 1383.13p 0.292929v 1394.92p 0.261195v 1420.87p 0.197482v 1449.18p 0.135412v 1482.2p 0.07v 1504.61p 0.028712v 1520.84p 0v)
* Driver
XI19 VSS VDD I19:A I19:Y INVx8_ASAP7_75t_R
* NET
C0 I19:Y VSS 31.6FF 
C1 I20:A VSS 31.6FF 
R0 I19:Y I20:A 3420
* Load
XI20 VSS VDD I20:A I20:Y BUFx3_ASAP7_75t_R
* Load parasitics
Cnear I20:Y 0 55.9049f
.tran 0.1p 2020.84p 0 0.1p
.measure tran fall_delay trig v(I19:A)=0.35v rise=1 targ v(I19:Y)=0.35v fall=1
.measure tran rise_delay trig v(I19:A)=0.35v fall=1 targ v(I19:Y)=0.35v rise=1
.measure tran rise_slew trig v(I19:Y)=0.07v rise=1 targ v(I19:Y)=0.63 rise=1
.measure tran fall_slew trig v(I19:Y)=0.63v fall=1 targ v(I19:Y)=0.07 fall=1
.end
