* SPICE circuit for net_5
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I11:A 0 pwl(0p 0v 261.788p 0v 276.068p 0.021v 285.587p 0.07v 293.917p 0.111121v 303.436p 0.15489v 312.956p 0.195562v 322.475p 0.233459v 331.995p 0.26887v 342.705p 0.306056v 364.124p 0.373242v 376.023p 0.407071v 387.923p 0.438805v 414.101p 0.502518v 442.66p 0.564588v 475.979p 0.63v 498.588p 0.671288v 514.955p 0.7v 1276.74p 0.7v 1291.02p 0.679v 1300.54p 0.63v 1308.87p 0.588879v 1318.39p 0.54511v 1327.91p 0.504438v 1337.43p 0.466541v 1346.95p 0.43113v 1357.66p 0.393944v 1379.08p 0.326758v 1390.98p 0.292929v 1402.88p 0.261195v 1429.06p 0.197482v 1457.62p 0.135412v 1490.93p 0.07v 1513.54p 0.028712v 1529.91p 0v)
* Driver
XI11 VSS VDD I11:A I11:Y INVx13_ASAP7_75t_R
* NET
C0 I11:Y VSS 80.9FF 
C1 I12:A VSS 80.9FF 
R0 I11:Y I12:A 1097
* Load
XI12 VSS VDD I12:A I12:Y BUFx4f_ASAP7_75t_R
* Load parasitics
Cnear I12:Y 0 127.161f
.tran 0.1p 2029.91p 0 0.1p
.measure tran fall_delay trig v(I11:A)=0.35v rise=1 targ v(I11:Y)=0.35v fall=1
.measure tran rise_delay trig v(I11:A)=0.35v fall=1 targ v(I11:Y)=0.35v rise=1
.measure tran rise_slew trig v(I11:Y)=0.07v rise=1 targ v(I11:Y)=0.63 rise=1
.measure tran fall_slew trig v(I11:Y)=0.63v fall=1 targ v(I11:Y)=0.07 fall=1
.end
