
#include "NAUtility/NAMemory.h"
#include "PLParam.h"



struct PLParam {
  double value;
  double min;
  double max;
};



PLParam* plAllocParam(void) {
  PLParam* param = naAlloc(PLParam);
  
  param->value = 0.;
  param->min = -1.;
  param->max = +1.;

  return param;
}



void plDeallocParam(PLParam* param) {
  naFree(param);
}



double plGetParamValue(const PLParam* param) {
  return param->value;
}
void plSetParamValue(PLParam* param, double value) {
  param->value = value;
}



double plGetParamMin(const PLParam* param) {
  return param->min;
}
void plSetParamMin(PLParam* param, double min) {
  param->min = min;
}



double plGetParamMax(const PLParam* param) {
  return param->max;
}
void plSetParamMax(PLParam* param, double max) {
  param->max = max;
}

