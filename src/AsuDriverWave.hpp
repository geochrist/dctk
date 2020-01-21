#pragma once
//
// Code to generate an exponential representative of what is used in the ASU 7nm PDK
//

#include <vector>
#include <math.h>

class AsuDriverWave {
public:
  //NOTE: delay is not exactly 50% of waveform, but will be close
  AsuDriverWave(long double trans, long double delay, long double vdd=0.70, 
                long double slew_thres_high = 90.0,
                long double slew_thres_low = 10.0)
    : trans_(trans), delay_(delay), vdd_(vdd),
      slew_thres_high_(slew_thres_high),
      slew_thres_low_(slew_thres_low),
      voltages_{0.0, 0.021, 0.07, 0.111121, 0.15489, 0.195562, 0.233459, 0.26887, 0.306056, 0.373242, 0.407071, 0.438805, 0.502518, 0.564588, 0.63, 0.671288, 0.7}
  {

      // default assumes 0.7v, so we just scale accordingly for larger voltages
      long double scale = vdd / 0.70 ;

      size_t n = voltages_.size();
      for (size_t i = 0; i < n; i++) {
          voltages_[i] *= scale;
      }

  }

  std::vector<long double> voltages() {
    return voltages_;
  }

  std::vector<long double> times() {
    std::vector<long double> ret(voltages_.size());

    for(size_t i=0; i<voltages_.size(); ++i) {
      ret[i] = t_from_v(voltages_[i]);
    }  

    return ret;
  }

  long double t_from_v(long double vTarget) {
   
    long double t_left = delay_ - trans_*2.0;
    //long double v_left = predriver_v_t(t_left, false);

    long double t_right = delay_ + trans_*2.0;
    //long double v_right = predriver_v_t(t_right, false);

    while (fabsl(t_right - t_left) > 1e-6) {
      long double t_mid = (t_left + t_right) * 0.5;
      long double v_mid = predriver_v_t(t_mid, false);
     
      if(v_mid < vTarget) {
        t_left = t_mid; 
        //v_left = v_mid;
      } else if(v_mid > vTarget) {
        t_right = t_mid; 
        //v_right = v_mid;
      } else {
        return t_mid;
      }
    }

    return (t_right + t_left) * 0.5; 
  }

  long double t_from_v_newton(long double vTarget) {
    //Using Newton's method

    size_t n_iter = 0; 
    long double t = delay_;
    long double v = predriver_v_t(t, false) - vTarget;

    //printf("Iteration t=%Lg v=%Lg\n", t, v);

    while (fabsl(v) > 1e-6) {
      long double t1 = delay_ + 1e-6;
      long double v1 = predriver_v_t(t1, false) - vTarget;
  
      long double deriv = (v1-v)/(t1-t);

      t = t - v / deriv;
      v = predriver_v_t(t, false) - vTarget;
      //printf("Iteration t=%Lg v=%Lg\n", t, v);

      ++n_iter;

      if(n_iter > 100) 
        return -9999.9;
    }
 
    return t;
  }

  long double predriver_v_t(long double t, bool clamp=true) {
    long double v_ramp = ramp_v_t(t, clamp);
    long double v_exp  = exp_v_t(t, true, clamp);
   
    long double v = 0.5*(v_ramp + v_exp);

    if(!clamp || (v < vdd_))
      return v;
    else
      return vdd_;
  }

  long double ramp_v_t(long double t, bool clamp=true) {
    // linear ramp (where slew is in percentage)
    //  v(t) = vdd * (slew_thres_high - slew_thres_low) * 0.01 / (trans) * (t  - delta)
    
    long double delta = delay_ - 0.5 * trans_ * 100.0 / (slew_thres_high_ - slew_thres_low_); 
    long double v = vdd_ * (slew_thres_high_ - slew_thres_low_) * (t - delta) * 0.01 / trans_;

    if(!clamp || (v > 0.0))
      return v;
    else
      return 0;
  }

  long double exp_v_t(long double t, bool match_trans=true, bool clamp=true) {
    // exponential RC (where slew is in percentage)
    // v(t) = vdd * (1-exp(- (t-delta) /tau)) 
    // tau = trans / ln ( slew_thres_high / slew_thres_low)

    // Two options for choosing delta
    //  1. delta is chosen so that transition trip points matches that of the ramp (thres=slew_thres-low)
    //    delta = (delay - (50 - thres) * trans / (slew_thres_high - slew_thres_low)) + tau * (1 - thres/100)
    //  2. delta is chosen so that delay trip points matches that of the ramp (thres=50)
    //       delta = delay + tau * ln(0.5)

    long double thres = 50.0;
    if(match_trans)
      thres = slew_thres_low_;

    long double tau = trans_ / logl(slew_thres_high_ / slew_thres_low_);

    long double delta_thres = slew_thres_high_ - slew_thres_low_;
    long double delta = (delay_ - (50.0 - thres) * trans_ / delta_thres) + tau * logl(1.0 - thres/100.0);

    long double v = vdd_ * (1.0 - expl(-1.0 * (t-delta) / tau));

    if(v >= 0.0)
      return v;
    else 
      return 0;
  }

private:
  long double trans_;
  long double delay_;
  long double vdd_;
  long double slew_thres_high_;
  long double slew_thres_low_;

  std::vector<long double> voltages_;
};
