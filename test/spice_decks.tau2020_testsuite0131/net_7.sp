* SPICE circuit for net_7
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I15:A 0 pwl(0p 0v 139.757p 0v 147.38p 0.021v 152.462p 0.07v 156.909p 0.111121v 161.991p 0.15489v 167.073p 0.195562v 172.155p 0.233459v 177.237p 0.26887v 182.954p 0.306056v 194.389p 0.373242v 200.741p 0.407071v 207.094p 0.438805v 221.07p 0.502518v 236.316p 0.564588v 254.103p 0.63v 266.173p 0.671288v 274.911p 0.7v 914.668p 0.7v 922.291p 0.679v 927.373p 0.63v 931.82p 0.588879v 936.902p 0.54511v 941.984p 0.504438v 947.066p 0.466541v 952.148p 0.43113v 957.865p 0.393944v 969.3p 0.326758v 975.652p 0.292929v 982.005p 0.261195v 995.981p 0.197482v 1011.23p 0.135412v 1029.01p 0.07v 1041.08p 0.028712v 1049.82p 0v)
* Driver
XI15 VSS VDD I15:A I15:Y HB3xp67_ASAP7_75t_R
* NET
C0 I15:Y VSS 6.23125FF 
C1 I16:A VSS 6.23125FF 
R0 I15:Y I16:A 441.781
* Load
XI16 VSS VDD I16:A I16:Y BUFx5_ASAP7_75t_R
* Load parasitics
Cnear I16:Y 0 14.0781f
.tran 0.1p 1549.82p 0 0.1p
.measure tran rise_delay trig v(I15:A)=0.35v rise=1 targ v(I15:Y)=0.35 rise=1
.measure tran fall_delay trig v(I15:A)=0.35v fall=1 targ v(I15:Y)=0.35v fall=1
.measure tran rise_slew trig v(I15:Y)=0.07v rise=1 targ v(I15:Y)=0.63 rise=1
.measure tran fall_slew trig v(I15:Y)=0.63v fall=1 targ v(I15:Y)=0.07 fall=1
.end
