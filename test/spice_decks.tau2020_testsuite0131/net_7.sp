* SPICE circuit for net_7
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I15:A 0 pwl(0p 0v 115.241p 0v 121.527p 0.021v 125.718p 0.07v 129.385p 0.111121v 133.575p 0.15489v 137.766p 0.195562v 141.956p 0.233459v 146.147p 0.26887v 150.861p 0.306056v 160.29p 0.373242v 165.528p 0.407071v 170.767p 0.438805v 182.291p 0.502518v 194.862p 0.564588v 209.53p 0.63v 219.482p 0.671288v 226.687p 0.7v 841.929p 0.7v 848.215p 0.679v 852.405p 0.63v 856.072p 0.588879v 860.262p 0.54511v 864.453p 0.504438v 868.644p 0.466541v 872.834p 0.43113v 877.549p 0.393944v 886.978p 0.326758v 892.216p 0.292929v 897.454p 0.261195v 908.978p 0.197482v 921.55p 0.135412v 936.217p 0.07v 946.17p 0.028712v 953.375p 0v)
* Driver
XI15 VSS VDD I15:A I15:Y HB3xp67_ASAP7_75t_R
* NET
C0 I15:Y VSS 6.925FF 
C1 I16:A VSS 6.925FF 
R0 I15:Y I16:A 1625
* Load
XI16 VSS VDD I16:A I16:Y HB3xp67_ASAP7_75t_R
* Load parasitics
Cnear I16:Y 0 13.6258f
.tran 0.1p 1453.37p 0 0.1p
.measure tran rise_delay trig v(I15:A)=0.35v rise=1 targ v(I15:Y)=0.35 rise=1
.measure tran fall_delay trig v(I15:A)=0.35v fall=1 targ v(I15:Y)=0.35v fall=1
.measure tran rise_slew trig v(I15:Y)=0.07v rise=1 targ v(I15:Y)=0.63 rise=1
.measure tran fall_slew trig v(I15:Y)=0.63v fall=1 targ v(I15:Y)=0.07 fall=1
.end
