
#include "NAUtility/NAMemory.h"
#include "NAUtility/NAString.h"
#include "PLParam.h"



struct PLParam {
  NAUTF8Char* name;
  double value;
  double min;
  double max;
  NABool integerOnly;
};



PLParam* plAllocParam(void) {
  PLParam* param = naAlloc(PLParam);
  
  param->name = NA_NULL;
  param->value = 0.;
  param->min = -1.;
  param->max = +1.;
  param->integerOnly = NA_FALSE;

  return param;
}



void plDeallocParam(PLParam* param) {
  naFree(param);
}



const NAUTF8Char* plGetParamName(const PLParam* param) {
  return param->name;
}
void plSetParamName(PLParam* param, const NAUTF8Char* name) {
  if(param->name) {
    naFree(param->name);
    param->name = NA_NULL;
  }
  if(name) {
    param->name = naAllocSprintf(NA_FALSE, "%s", name);
  }
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



NABool plGetParamIntegerOnly(const PLParam* param) {
  return param->integerOnly;
}
void plSetParamIntegerOnly(PLParam* param, NABool integerOnly) {
  param->integerOnly = integerOnly;
}
