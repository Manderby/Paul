
#include "PlottyUser.h"
#include "Function.h"



double evaluate(double t, const double* p) {
  return 10. * p[0] * sin(t) / t;
  //return params[0] * 10. * sin(t);
//  return p[0] * t*t*t + p[1] * t*t + p[2] * t + p[3];
}



void plottyUserStartup(void) {
  // Insert function definition here. Have fun!
  
  Function* fun = getGlobalFunction();
  
  setFunctionEvaluateFunction(fun, evaluate);
  
  addFunctionParameter(fun, -10., 10., 1.);
//  addFunctionParameter(fun, -1., 1., 0.);
//  addFunctionParameter(fun, -1., 1., 0.);
//  addFunctionParameter(fun, -1., 1., 0.);
}



void plottyUserShutdown(void) {
  // Insert necessary cleanup.
}
