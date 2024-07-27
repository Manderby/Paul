
#include "NAUtility/NAMemory.h"
#include "Param.h"



struct Param {
  double value;
  double min;
  double max;
};



Param* allocParam(void) {
  Param* param = naAlloc(Param);
  
  param->value = 0.;
  param->min = -1.;
  param->max = +1.;

  return param;
}



void deallocParam(Param* con) {
  // do nothing
}



double getParamValue(const Param* param) {
  return param->value;
}
void setParamValue(Param* param, double value) {
  param->value = value;
}



double getParamMin(const Param* param) {
  return param->min;
}
void setParamMin(Param* param, double min) {
  param->min = min;
}



double getParamMax(const Param* param) {
  return param->max;
}
void setParamMax(Param* param, double max) {
  param->max = max;
}

