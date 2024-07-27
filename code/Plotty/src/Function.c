
#include "NAUtility/NAMemory.h"
#include "NAStruct/NAStack.h"
#include "Function.h"
#include "Param.h"



struct Function {
  size_t paramCount;
  Param** params;
  EvaluateFunction eval;
};



Function* allocFunction(void) {
  Function* fun = naAlloc(Function);

  fun->paramCount = 0;
  fun->params = NA_NULL;
  fun->eval = NA_NULL;

  return fun;
}

void deallocFunction(Function* fun) {
  for(size_t i = 0; i < fun->paramCount; ++i) {
    deallocParam(fun->params[i]);
  }
  naFree(fun->params);

  naFree(fun);
}



void setFunctionEvaluateFunction(
  Function* fun,
  EvaluateFunction eval)
{
  fun->eval = eval;
}



size_t addFunctionParameter(Function* fun, double min, double max, double value) {
  fun->paramCount++;
  
  Param** newParams = naMalloc(sizeof(Param*) * fun->paramCount);
  if(fun->params){
    naCopyn(newParams, fun->params, sizeof(Param*) * (fun->paramCount - 1));
    naFree(fun->params);
  }
  fun->params = newParams;
  
  Param* param = allocParam();
  setParamMin(param, min);
  setParamMax(param, max);
  setParamValue(param, value);

  fun->params[fun->paramCount - 1] = param;

  return index;
}



double evaluateFunction(
  const Function* fun,
  double t,
  double* params)
{
  return fun->eval(t, params);
}



size_t getFunctionParamCount(Function* fun) {
  return fun->paramCount;
}

Param* getFunctionParameter(Function* fun, size_t index) {
  return fun->params[index];
}
