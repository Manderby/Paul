
#include "main.h"
PL_PROTOTYPE(PLParam);



typedef double (*PLEvaluator)(double t, const double* params);
typedef struct PLFunction PLFunction;



// Allocate and deallocate functions
PLFunction* plAllocFunction(void);
void plDeallocFunction(PLFunction* func);

// Set the evaluator function callback.
void plSetFunctionEvaluator(
  PLFunction* func,
  PLEvaluator eval);

// Add a parameter
size_t plAddFunctionParameter(
  PLFunction* func,
  double min,
  double max,
  double value);

// Getter
size_t plGetFunctionParamCount(
  const PLFunction* func);
PLParam* plGetFunctionParameter(
  const PLFunction* func,
  size_t index);

// evaluates the fuction at the given t with the given parameters
double plEvaluateFunction(
  const PLFunction* func,
  double t,
  double* params);

