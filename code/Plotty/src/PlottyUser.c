
#include "PlottyUser.h"
#include "FunctionDefinition.h"



void plottyUserStartup(void) {
  // Insert function definition here. Have fun!
  
  FunctionDefinition* fun = getGlobalFunctionDefinition();
  
  addFunctionDefinitionParameter(fun, -10., 10., 1.);
  addFunctionDefinitionParameter(fun, -1., 1., 0.);
  addFunctionDefinitionParameter(fun, -1., 1., 0.);
  addFunctionDefinitionParameter(fun, -1., 1., 0.);
}



void plottyUserShutdown(void) {
  // Insert necessary cleanup.
}
