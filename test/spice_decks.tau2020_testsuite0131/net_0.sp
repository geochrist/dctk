* SPICE circuit for net_0
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I1:A 0 pwl(0p 0v 348.866p 0v 367.895p 0.021v 380.581p 0.07v 391.681p 0.111121v 404.367p 0.15489v 417.053p 0.195562v 429.739p 0.233459v 442.425p 0.26887v 456.697p 0.306056v 485.24p 0.373242v 501.098p 0.407071v 516.956p 0.438805v 551.842p 0.502518v 589.9p 0.564588v 634.301p 0.63v 664.431p 0.671288v 686.242p 0.7v 1535.11p 0.7v 1554.14p 0.679v 1566.82p 0.63v 1577.92p 0.588879v 1590.61p 0.54511v 1603.3p 0.504438v 1615.98p 0.466541v 1628.67p 0.43113v 1642.94p 0.393944v 1671.48p 0.326758v 1687.34p 0.292929v 1703.2p 0.261195v 1738.08p 0.197482v 1776.14p 0.135412v 1820.54p 0.07v 1850.67p 0.028712v 1872.48p 0v)
* Driver
XI1 VSS VDD I1:A I1:Y BUFx4f_ASAP7_75t_R
* NET
C0 I1:Y VSS 7.1FF 
C1 I2:A VSS 7.1FF 
R0 I1:Y I2:A 451.5
* Load
XI2 VSS VDD I2:A I2:Y BUFx10_ASAP7_75t_R
* Load parasitics
Cnear I2:Y 0 214.092f
.tran 0.1p 2372.49p 0 0.1p
.measure tran rise_delay trig v(I1:A)=0.35v rise=1 targ v(I1:Y)=0.35 rise=1
.measure tran fall_delay trig v(I1:A)=0.35v fall=1 targ v(I1:Y)=0.35v fall=1
.measure tran rise_slew trig v(I1:Y)=0.07v rise=1 targ v(I1:Y)=0.63 rise=1
.measure tran fall_slew trig v(I1:Y)=0.63v fall=1 targ v(I1:Y)=0.07 fall=1
.end
