
#include "PLFunction.h"

#include "PLParam.h"

#include "NAUtility/NAString.h"
#include "NAUtility/NAMemory.h"
#include "NAStruct/NAStack.h"



struct PLFunction {
  NAUTF8Char* name;
  size_t paramCount;
  PLParam** params;
  PLEvaluator eval;
  NABool integerOnly;
  double minBound;
  double maxBound;
};



PLFunction* plAllocFunction(void) {
  PLFunction* func = naAlloc(PLFunction);

  func->name = NA_NULL;
  func->paramCount = 0;
  func->params = NA_NULL;
  func->eval = NA_NULL;
  func->integerOnly = NA_FALSE;
  func->minBound = -NA_INFINITY;
  func->maxBound = +NA_INFINITY;

  return func;
}

void plDeallocFunction(PLFunction* func) {
  for(size_t i = 0; i < func->paramCount; ++i) {
    plDeallocParam(func->params[i]);
  }
  naFree(func->params);

  naFree(func);
}



void plSetFunctionEvaluator(
  PLFunction* func,
  PLEvaluator eval)
{
  func->eval = eval;
}



void plSetFunctionName(PLFunction* func, const NAUTF8Char* name) {
  if(func->name) {
    naFree(func->name);
    func->name = NA_NULL;
  }
  if(name) {
    func->name = naAllocSprintf(NA_FALSE, "%s", name);
  }
}



NABool plGetFunctionIntegerOnly(const PLFunction* func) {
  return func->integerOnly;
}
void plSetFunctionIntegerOnly(PLFunction* func, NABool integerOnly) {
  func->integerOnly = integerOnly;
}



double plGetFunctionMinBound(const PLFunction* func) {
  return func->minBound;
}
double plGetFunctionMaxBound(const PLFunction* func) {
  return func->maxBound;
}
void plSetFunctionBounds(
  PLFunction* func,
  double min,
  double max)
{
  func->minBound = min;
  func->maxBound = max;
}



const NAUTF8Char* plGetFunctionName(const PLFunction* func) {
  return func->name;
}




size_t plAddFunctionParameter(PLFunction* func, const NAUTF8Char* name, double min, double max, double value, NABool integerOnly) {
  func->paramCount++;
  
  PLParam** newParams = naMalloc(sizeof(PLParam*) * func->paramCount);
  if(func->params){
    naCopyn(newParams, func->params, sizeof(PLParam*) * (func->paramCount - 1));
    naFree(func->params);
  }
  func->params = newParams;
  
  PLParam* param = plAllocParam();
  plSetParamName(param, name);
  plSetParamMin(param, min);
  plSetParamMax(param, max);
  plSetParamValue(param, value);
  plSetParamIntegerOnly(param, integerOnly);

  func->params[func->paramCount - 1] = param;

  return func->paramCount - 1;
}



double plEvaluateFunction(
  const PLFunction* func,
  double t,
  double* params)
{
  return func->eval(t, params);
}



size_t plGetFunctionParamCount(const PLFunction* func) {
  return func->paramCount;
}

PLParam* plGetFunctionParameter(
  const PLFunction* func,
  size_t index)
{
  return func->params[index];
}
