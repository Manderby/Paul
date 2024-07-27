
#include "NAUtility/NAMemory.h"
#include "NAStruct/NAStack.h"
#include "FunctionDefinition.h"
#include "Param.h"



struct FunctionDefinition {
  size_t paramCount;
  Param** params;
};



FunctionDefinition* allocFunctionDefinition(void) {
  FunctionDefinition* fun = naAlloc(FunctionDefinition);

  fun->paramCount = 0;
  fun->params = NA_NULL;

  return fun;
}

void deallocFunctionDefinition(FunctionDefinition* fun) {
  for(size_t i = 0; i < fun->paramCount; ++i) {
    deallocParam(fun->params[i]);
  }
  naFree(fun->params);

  naFree(fun);
}



size_t addFunctionDefinitionParameter(FunctionDefinition* fun, double min, double max, double value) {
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



Param* getFunctionDefinitionParameter(FunctionDefinition* fun, size_t index) {
  return fun->params[index];
}
