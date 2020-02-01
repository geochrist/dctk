* SPICE circuit for net_6
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I13:A 0 pwl(0p 0v 211.534p 0v 223.072p 0.021v 230.764p 0.07v 237.495p 0.111121v 245.187p 0.15489v 252.879p 0.195562v 260.571p 0.233459v 268.263p 0.26887v 276.917p 0.306056v 294.224p 0.373242v 303.839p 0.407071v 313.455p 0.438805v 334.608p 0.502518v 357.684p 0.564588v 384.607p 0.63v 402.876p 0.671288v 416.101p 0.7v 1127.63p 0.7v 1139.17p 0.679v 1146.87p 0.63v 1153.6p 0.588879v 1161.29p 0.54511v 1168.98p 0.504438v 1176.67p 0.466541v 1184.36p 0.43113v 1193.02p 0.393944v 1210.33p 0.326758v 1219.94p 0.292929v 1229.56p 0.261195v 1250.71p 0.197482v 1273.79p 0.135412v 1300.71p 0.07v 1318.98p 0.028712v 1332.2p 0v)
* Driver
XI13 VSS VDD I13:A I13:Y BUFx24_ASAP7_75t_R
* NET
C0 I13:Y VSS 21.675FF 
C1 I14:A VSS 21.675FF 
R0 I13:Y I14:A 29100
* Load
XI14 VSS VDD I14:A I14:Y INVx11_ASAP7_75t_R
* Load parasitics
Cnear I14:Y 0 78.8093f
.tran 0.1p 1832.2p 0 0.1p
.measure tran rise_delay trig v(I13:A)=0.35v rise=1 targ v(I13:Y)=0.35 rise=1
.measure tran fall_delay trig v(I13:A)=0.35v fall=1 targ v(I13:Y)=0.35v fall=1
.measure tran rise_slew trig v(I13:Y)=0.07v rise=1 targ v(I13:Y)=0.63 rise=1
.measure tran fall_slew trig v(I13:Y)=0.63v fall=1 targ v(I13:Y)=0.07 fall=1
.end
