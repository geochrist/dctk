* SPICE circuit for net_1
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I3:A 0 pwl(0p 0v 392.879p 0v 414.308p 0.021v 428.595p 0.07v 441.096p 0.111121v 455.382p 0.15489v 469.668p 0.195562v 483.955p 0.233459v 498.241p 0.26887v 514.314p 0.306056v 546.458p 0.373242v 564.316p 0.407071v 582.175p 0.438805v 621.462p 0.502518v 664.322p 0.564588v 714.325p 0.63v 748.255p 0.671288v 772.819p 0.7v 1665.7p 0.7v 1687.13p 0.679v 1701.41p 0.63v 1713.91p 0.588879v 1728.2p 0.54511v 1742.49p 0.504438v 1756.77p 0.466541v 1771.06p 0.43113v 1787.13p 0.393944v 1819.28p 0.326758v 1837.14p 0.292929v 1854.99p 0.261195v 1894.28p 0.197482v 1937.14p 0.135412v 1987.14p 0.07v 2021.07p 0.028712v 2045.64p 0v)
* Driver
XI3 VSS VDD I3:A I3:Y BUFx2_ASAP7_75t_R
* NET
C0 I3:Y VSS 41.2FF 
C1 I4:A VSS 41.2FF 
R0 I3:Y I4:A 8480
* Load
XI4 VSS VDD I4:A I4:Y BUFx24_ASAP7_75t_R
* Load parasitics
Cnear I4:Y 0 246.906f
.tran 0.1p 2545.64p 0 0.1p
.measure tran rise_delay trig v(I3:A)=0.35v rise=1 targ v(I3:Y)=0.35 rise=1
.measure tran fall_delay trig v(I3:A)=0.35v fall=1 targ v(I3:Y)=0.35v fall=1
.measure tran rise_slew trig v(I3:Y)=0.07v rise=1 targ v(I3:Y)=0.63 rise=1
.measure tran fall_slew trig v(I3:Y)=0.63v fall=1 targ v(I3:Y)=0.07 fall=1
.end
