* SPICE circuit for net_8
.options device temp=25
.options device tnom=25
.include ../../ASAP7_PDKandLIB_v1p6/lib_release_191006/asap7_7p5t_library/rev25/CDL/xAct3D_extracted/Extracted_netlists/asap7sc7p5t_INVBUF_RVT.sp.modified
.include ../../ASAP7_PDKandLIB_v1p6/asap7PDK_r1p6/models/hspice/7nm_TT.pm.modified
* Constant Voltage Sources
VVDD VDD 0 0.7
VVSS VSS 0 0
Vinput I17:A 0 pwl(0p 0v 126.589p 0v 133.494p 0.021v 138.097p 0.07v 142.125p 0.111121v 146.728p 0.15489v 151.332p 0.195562v 155.935p 0.233459v 160.538p 0.26887v 165.717p 0.306056v 176.074p 0.373242v 181.828p 0.407071v 187.582p 0.438805v 200.241p 0.502518v 214.051p 0.564588v 230.162p 0.63v 241.095p 0.671288v 249.009p 0.7v 875.599p 0.7v 882.503p 0.679v 887.107p 0.63v 891.135p 0.588879v 895.738p 0.54511v 900.341p 0.504438v 904.944p 0.466541v 909.547p 0.43113v 914.726p 0.393944v 925.083p 0.326758v 930.837p 0.292929v 936.592p 0.261195v 949.25p 0.197482v 963.06p 0.135412v 979.172p 0.07v 990.104p 0.028712v 998.019p 0v)
* Driver
XI17 VSS VDD I17:A I17:Y INVx2_ASAP7_75t_R
* NET
C0 I17:Y VSS 31.7FF 
C1 I18:A VSS 31.7FF 
R0 I17:Y I18:A 1074.5
* Load
XI18 VSS VDD I18:A I18:Y INVx3_ASAP7_75t_R
* Load parasitics
Cnear I18:Y 0 8.16891f
.tran 0.1p 1498.02p 0 0.1p
.measure tran fall_delay trig v(I17:A)=0.35v rise=1 targ v(I17:Y)=0.35v fall=1
.measure tran rise_delay trig v(I17:A)=0.35v fall=1 targ v(I17:Y)=0.35v rise=1
.measure tran rise_slew trig v(I17:Y)=0.07v rise=1 targ v(I17:Y)=0.63 rise=1
.measure tran fall_slew trig v(I17:Y)=0.63v fall=1 targ v(I17:Y)=0.07 fall=1
.end
