
#include "PLFunction.h"
#include "PLApplication.h"
#include "NAMath/NAMathOperators.h"
#include <stdio.h>


#define SCALE 20.
#define MAX_TONE 100


double evaluate(double t, double s, const double* params) {
  double linearDistance = log(t) / log(params[2]);
  double rounded = naRound(linearDistance * s) / s;
  double simpleError = linearDistance - rounded;
  
  double errorScale = SCALE * s * s;
  return errorScale * simpleError * simpleError;
}



double evaluate2(double t, const double* params) {
  double totalError = 0.;
  double totalErrorWeight = 0.;

  for(size_t i = 1; i < MAX_TONE; i += 1) {
    //if(i == 7 || i == 11) // Alphorn
    if(i == 3 || i == 5 || i == 6) // 5th, 4th, 3rd      // 12 TET 1.998294 -> error 0.093562
                                                         // 19 TET 2.003962 -> error 0.011330
    //if(i == 3 || i == 5 || i == 6 || i == 9 || i == 15)  // 12 TET 1.998624 -> error 0.097749
                                                         // 19 TET 2.004320 -> error 0.015086
    {
      double errorWeight = 1. / i;
      totalError += errorWeight * evaluate(i, t, params);
      totalErrorWeight += errorWeight;
    }
  }
  
//  if(t == 19)
//    printf("%f\n", totalError / totalErrorWeight);
  return totalError / totalErrorWeight / SCALE * 60.;
}



void plUserStartup(void) {
  PLFunction* func = plAllocFunction();
  
  plSetFunctionName(func, "TET");
  plSetFunctionEvaluator(func, evaluate2);
  plSetFunctionIntegerOnly(func, NA_FALSE);
  plSetFunctionBounds(func, 1., NA_INFINITY);

  plAddFunctionParameter(func, "s", -10., 10., 1., NA_FALSE);
  plAddFunctionParameter(func, "m", 1., MAX_TONE, 10., NA_FALSE);
  plAddFunctionParameter(func, "O", 1.9, 2.1, 2., NA_FALSE);

  plRegisterFunction(func);
}



void plUserShutdown(void) {
}
