
#include "Application.h"

#include "FunctionDefinition.h"
#include "Param.h"
#include "PlottyUser.h"
#include "WindowController.h"

#include "NAApp/NAApp.h"



struct Application {
  FunctionDefinition* fun;

  NAFont* mathFont;
  WindowController* windowController;
};

Application* _app = NA_NULL;



Application* allocApplication(void) {
  Application* app = naAlloc(Application);

  app->fun = allocFunctionDefinition();

  app->mathFont = NA_NULL;
  app->windowController = NA_NULL;

  return app;
}



void startupApplicationGUI(Application* app) {
  app->mathFont = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  app->windowController = allocWindowController();
}



void deallocApplication(Application* app) {
  deallocFunctionDefinition(app->fun);
  deallocWindowController(app->windowController);
  naRelease(app->mathFont);

//  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
//    deallocParam(app->params[i]);
//  }
  
  naFree(app);
}



void startupApplication(void* arg) {
  _app = allocApplication();
}

void startupGUI(void* arg) {
  plottyUserStartup();
  startupApplicationGUI(_app);
}

void shutdownApplication(void* arg) {
  plottyUserShutdown();
  deallocApplication(_app);
}



FunctionDefinition* getGlobalFunctionDefinition() {
  return _app->fun;
}

NAFont* getGlobalMathFont() {
  return _app->mathFont;
}

Param* getGlobalParam(size_t index) {
  return getFunctionDefinitionParameter(_app->fun, index);
}

void drawGlobalScene() {
  updateWindowControllerScene(_app->windowController);
}
