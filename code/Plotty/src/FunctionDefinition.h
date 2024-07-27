
#include "main.h"
PLOTTY_PROTOTYPE(Param);

typedef struct FunctionDefinition FunctionDefinition;

FunctionDefinition* allocFunctionDefinition(void);
void deallocFunctionDefinition(FunctionDefinition* con);

size_t addFunctionDefinitionParameter(
  FunctionDefinition* fun,
  double min,
  double max,
  double value);

Param* getFunctionDefinitionParameter(FunctionDefinition* fun, size_t index);
