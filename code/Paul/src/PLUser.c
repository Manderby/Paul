
#include "PLFunction.h"

// todo: try to remove this include.
#include "PLApplication.h"



double evaluate(double t, const double* p) {
  return p[0] * t*t*t + p[1] * t*t + p[2] * t + p[3];
}

double evaluate2(double t, const double* p) {
  return 10. * p[0] * sin(t) / t;
}



// Define Functions, Parameters, Settings you'd like to see in the application.
// This function will be called automatically upon application startup.
void plUserStartup(void) {
  PLFunction* func = plAllocFunction();
  
  plSetFunctionName(func, "Cubic function");
  plSetFunctionEvaluator(func, evaluate);
  plAddFunctionParameter(func, "a", -1., 1., -3., NA_FALSE);
  plAddFunctionParameter(func, "b", -1., 1., 0., NA_FALSE);
  plAddFunctionParameter(func, "c", -1., 1., 2., NA_FALSE);
  plAddFunctionParameter(func, "d", -1., 1., 0., NA_FALSE);
  
  plRegisterFunction(func);


  PLFunction* func2 = plAllocFunction();
  plSetFunctionBounds(func2, 0., 10.);
  plSetFunctionIntegerOnly(func2, NA_TRUE);

  plSetFunctionName(func2, "Sinc Function");
  plSetFunctionEvaluator(func2, evaluate2);
  plAddFunctionParameter(func2, "A", -10., 10., 1., NA_TRUE);
  
  plRegisterFunction(func2);
}



// Cleanup everything you set up in the plUserStartup function.
// This function will be called when the application quits.
void plUserShutdown(void) {
  // todo
}
