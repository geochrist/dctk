* SPICE circuit for net_8
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I17:A 0 pwl(0p 0v 315.709p 0v 332.929p 0.021v 344.41p 0.07v 354.455p 0.111121v 365.935p 0.15489v 377.416p 0.195562v 388.896p 0.233459v 400.376p 0.26887v 413.292p 0.306056v 439.122p 0.373242v 453.473p 0.407071v 467.823p 0.438805v 499.394p 0.502518v 533.835p 0.564588v 574.016p 0.63v 601.282p 0.671288v 621.021p 0.7v 1436.73p 0.7v 1453.95p 0.679v 1465.43p 0.63v 1475.48p 0.588879v 1486.96p 0.54511v 1498.44p 0.504438v 1509.92p 0.466541v 1521.4p 0.43113v 1534.31p 0.393944v 1560.14p 0.326758v 1574.49p 0.292929v 1588.84p 0.261195v 1620.42p 0.197482v 1654.86p 0.135412v 1695.04p 0.07v 1722.3p 0.028712v 1742.04p 0v)
* Driver
XI17 VSS VDD I17:A I17:Y INVx11_ASAP7_75t_R
* NET
C0 I17:Y VSS 90.4FF 
C1 I18:A VSS 90.4FF 
R0 I17:Y I18:A 1192
* Load
XI18 VSS VDD I18:A I18:Y INVx13_ASAP7_75t_R
* Load parasitics
Cnear I18:Y 0 694.271f
.tran 0.1p 2242.04p 0 0.1p
.measure tran fall_delay trig v(I17:A)=0.35v rise=1 targ v(I17:Y)=0.35v fall=1
.measure tran rise_delay trig v(I17:A)=0.35v fall=1 targ v(I17:Y)=0.35v rise=1
.measure tran rise_slew trig v(I17:Y)=0.07v rise=1 targ v(I17:Y)=0.63 rise=1
.measure tran fall_slew trig v(I17:Y)=0.63v fall=1 targ v(I17:Y)=0.07 fall=1
.end
