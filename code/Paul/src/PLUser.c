
#include "PLFunction.h"
#include "PLApplication.h"
#include "NAMath/NAMathOperators.h"



double evaluate(double t, const double* params) {
  return params[0] * t * t;
}



void plUserStartup(void) {
  PLFunction* func = plAllocFunction();
  
  plSetFunctionName(func, "TET");
  plSetFunctionEvaluator(func, evaluate);
  plSetFunctionIntegerOnly(func, NA_FALSE);
  plSetFunctionBounds(func, -NA_INFINITY, NA_INFINITY);

  plAddFunctionParameter(func, "s", -10., 10., 1., NA_FALSE);

  plRegisterFunction(func);
}



void plUserShutdown(void) {
}
