
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
};



PLFunction* plAllocFunction(void) {
  PLFunction* func = naAlloc(PLFunction);

  func->name = NA_NULL;
  func->paramCount = 0;
  func->params = NA_NULL;
  func->eval = NA_NULL;

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




const NAUTF8Char* plGetFunctionName(const PLFunction* func) {
  return func->name;
}




size_t plAddFunctionParameter(PLFunction* func, const NAUTF8Char* name, double min, double max, double value) {
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
