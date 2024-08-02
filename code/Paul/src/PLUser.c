
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
  plAddFunctionParameter(func, -1., 1., -3.);
  plAddFunctionParameter(func, -1., 1., 0.);
  plAddFunctionParameter(func, -1., 1., 2.);
  plAddFunctionParameter(func, -1., 1., 0.);
  
  plRegisterFunction(func);


  PLFunction* func2 = plAllocFunction();

  plSetFunctionName(func2, "Sinc Function");
  plSetFunctionEvaluator(func2, evaluate2);
  plAddFunctionParameter(func2, -10., 10., 1.);
  
  plRegisterFunction(func2);
}



// Cleanup everything you set up in the plUserStartup function.
// This function will be called when the application quits.
void plUserShutdown(void) {
  // todo
}
