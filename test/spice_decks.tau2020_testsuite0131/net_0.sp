* SPICE circuit for net_0
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I1:A 0 pwl(0p 0v 84.8367p 0v 89.4641p 0.021v 92.5491p 0.07v 95.2485p 0.111121v 98.3334p 0.15489v 101.418p 0.195562v 104.503p 0.233459v 107.588p 0.26887v 111.059p 0.306056v 118p 0.373242v 121.856p 0.407071v 125.713p 0.438805v 134.196p 0.502518v 143.451p 0.564588v 154.249p 0.63v 161.575p 0.671288v 166.88p 0.7v 751.716p 0.7v 756.344p 0.679v 759.429p 0.63v 762.128p 0.588879v 765.213p 0.54511v 768.298p 0.504438v 771.383p 0.466541v 774.468p 0.43113v 777.938p 0.393944v 784.88p 0.326758v 788.736p 0.292929v 792.592p 0.261195v 801.076p 0.197482v 810.331p 0.135412v 821.128p 0.07v 828.455p 0.028712v 833.759p 0v)
* Driver
XI1 VSS VDD I1:A I1:Y BUFx12_ASAP7_75t_R
* NET
C0 I1:Y VSS 80.8FF 
C1 I2:A VSS 80.8FF 
R0 I1:Y I2:A 1096
* Load
XI2 VSS VDD I2:A I2:Y HB4xp67_ASAP7_75t_R
* Load parasitics
Cnear I2:Y 0 10.3456f
.tran 0.1p 1333.76p 0 0.1p
.measure tran rise_delay trig v(I1:A)=0.35v rise=1 targ v(I1:Y)=0.35 rise=1
.measure tran fall_delay trig v(I1:A)=0.35v fall=1 targ v(I1:Y)=0.35v fall=1
.measure tran rise_slew trig v(I1:Y)=0.07v rise=1 targ v(I1:Y)=0.63 rise=1
.measure tran fall_slew trig v(I1:Y)=0.63v fall=1 targ v(I1:Y)=0.07 fall=1
.end
