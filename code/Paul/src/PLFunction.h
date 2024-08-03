
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

// Gets and sets the function name. Setting the name copies the string.
const NAUTF8Char* plGetFunctionName(
  const PLFunction* func);
void plSetFunctionName(
  PLFunction* func,
  const NAUTF8Char* name);
  
NABool plGetFunctionIntegerOnly(const PLFunction* func);
void plSetFunctionIntegerOnly(PLFunction* func, NABool integerOnly);

double plGetFunctionMinBound(const PLFunction* func);
double plGetFunctionMaxBound(const PLFunction* func);
void plSetFunctionBounds(
  PLFunction* func,
  double min,
  double max);
  
// Add a parameter
size_t plAddFunctionParameter(
  PLFunction* func,
  const NAUTF8Char* name,
  double min,
  double max,
  double value,
  NABool integerOnly);

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

