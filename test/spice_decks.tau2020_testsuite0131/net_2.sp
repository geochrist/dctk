* SPICE circuit for net_2
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I5:A 0 pwl(0p 0v 396.79p 0v 418.433p 0.021v 432.862p 0.07v 445.487p 0.111121v 459.916p 0.15489v 474.345p 0.195562v 488.773p 0.233459v 503.202p 0.26887v 519.434p 0.306056v 551.899p 0.373242v 569.935p 0.407071v 587.971p 0.438805v 627.65p 0.502518v 670.936p 0.564588v 721.437p 0.63v 755.705p 0.671288v 780.514p 0.7v 1677.3p 0.7v 1698.95p 0.679v 1713.38p 0.63v 1726p 0.588879v 1740.43p 0.54511v 1754.86p 0.504438v 1769.29p 0.466541v 1783.72p 0.43113v 1799.95p 0.393944v 1832.41p 0.326758v 1850.45p 0.292929v 1868.48p 0.261195v 1908.16p 0.197482v 1951.45p 0.135412v 2001.95p 0.07v 2036.22p 0.028712v 2061.03p 0v)
* Driver
XI5 VSS VDD I5:A I5:Y INVxp33_ASAP7_75t_R
* NET
C0 I5:Y VSS 5.59375FF 
C1 I6:A VSS 5.59375FF 
R0 I5:Y I6:A 293.238
* Load
XI6 VSS VDD I6:A I6:Y BUFx10_ASAP7_75t_R
* Load parasitics
Cnear I6:Y 0 119.229f
.tran 0.1p 2561.03p 0 0.1p
.measure tran fall_delay trig v(I5:A)=0.35v rise=1 targ v(I5:Y)=0.35v fall=1
.measure tran rise_delay trig v(I5:A)=0.35v fall=1 targ v(I5:Y)=0.35v rise=1
.measure tran rise_slew trig v(I5:Y)=0.07v rise=1 targ v(I5:Y)=0.63 rise=1
.measure tran fall_slew trig v(I5:Y)=0.63v fall=1 targ v(I5:Y)=0.07 fall=1
.end
