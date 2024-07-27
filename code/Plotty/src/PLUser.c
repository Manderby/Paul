
#include "PLFunction.h"

// todo: try to remove this include.
#include "PLApplication.h"



double evaluate(double t, const double* p) {
//  return 10. * p[0] * sin(t) / t;
  //return params[0] * 10. * sin(t);
  return p[0] * t*t*t + p[1] * t*t + p[2] * t + p[3];
}



// Define Functions, Parameters, Settings you'd like to see in the application.
// This function will be called automatically upon application startup.
void plUserStartup(void) {
  PLFunction* func = plGetGlobalFunction();
  
  plSetFunctionEvaluator(func, evaluate);
  
  plAddFunctionParameter(func, -1., 1., 0.);
  plAddFunctionParameter(func, -1., 1., 0.);
  plAddFunctionParameter(func, -1., 1., 0.);
  plAddFunctionParameter(func, -1., 1., 0.);
}



// Cleanup everything you set up in the plUserStartup function.
// This function will be called when the application quits.
void plUserShutdown(void) {
  // todo
}
