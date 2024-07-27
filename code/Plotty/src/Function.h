
#include "main.h"
PLOTTY_PROTOTYPE(Param);



typedef double (*EvaluateFunction)(double t, const double* p);
typedef struct Function Function;



Function* allocFunction(void);
void deallocFunction(Function* con);

void setFunctionEvaluateFunction(
  Function* fun,
  EvaluateFunction eval);

size_t addFunctionParameter(
  Function* fun,
  double min,
  double max,
  double value);

double evaluateFunction(
  const Function* fun,
  double t,
  double* params);

size_t getFunctionParamCount(Function* fun);
Param* getFunctionParameter(Function* fun, size_t index);
