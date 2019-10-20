#ifndef DCTK_PIMODEL_HPP_
#define DCTK_PIMODEL_HPP_

namespace dctk {

class PiModel {

public:

    PiModel() {
	_cnear = 0.0;
	_res = 0.0;
	_cfar = 0.0;
    }
    
    PiModel(float cnear, float res, float cfar) {
	_cnear = cnear;
	_res = res;
	_cfar = cfar;
    };
    
    void set_cnear(float f) { _cnear = f; }
    void set_res(float f) { _res = f; }
    void set_cfar(float f) { _cfar = f; }

    float get_cnear() { return _cnear; }
    float get_res() { return _res; }
    float get_cfar() { return _cfar; }

private:

    float _cnear;
    float _res;
    float _cfar;

};

}

#endif
