* SPICE circuit for net_2
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I5:A 0 pwl(0p 0v 245.053p 0v 258.42p 0.021v 267.331p 0.07v 275.128p 0.111121v 284.039p 0.15489v 292.95p 0.195562v 301.861p 0.233459v 310.772p 0.26887v 320.797p 0.306056v 340.847p 0.373242v 351.985p 0.407071v 363.124p 0.438805v 387.63p 0.502518v 414.362p 0.564588v 445.551p 0.63v 466.715p 0.671288v 482.036p 0.7v 1227.09p 0.7v 1240.46p 0.679v 1249.37p 0.63v 1257.16p 0.588879v 1266.08p 0.54511v 1274.99p 0.504438v 1283.9p 0.466541v 1292.81p 0.43113v 1302.83p 0.393944v 1322.88p 0.326758v 1334.02p 0.292929v 1345.16p 0.261195v 1369.67p 0.197482v 1396.4p 0.135412v 1427.59p 0.07v 1448.75p 0.028712v 1464.07p 0v)
* Driver
XI5 I5:A VSS VDD I5:Y BUFx12f_ASAP7_75t_R
* NET
C0 I5:Y VSS 89.2FF 
C1 I6:A VSS 89.2FF 
R0 I5:Y I6:A 2508
* Load
XI6 VSS VDD I6:A I6:Y BUFx12_ASAP7_75t_R
* Load parasitics
Cnear I6:Y 0 280.847f
.tran 0.1p 1964.07p 0 0.1p
.measure tran rise_delay trig v(I5:A)=0.35v rise=1 targ v(I5:Y)=0.35 rise=1
.measure tran fall_delay trig v(I5:A)=0.35v fall=1 targ v(I5:Y)=0.35v fall=1
.measure tran rise_slew trig v(I5:Y)=0.07v rise=1 targ v(I5:Y)=0.63 rise=1
.measure tran fall_slew trig v(I5:Y)=0.63v fall=1 targ v(I5:Y)=0.07 fall=1
.end
