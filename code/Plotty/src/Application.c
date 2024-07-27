
#include "Application.h"

#include "Function.h"
#include "Param.h"
#include "PlottyUser.h"
#include "WindowController.h"

#include "NAApp/NAApp.h"



struct Application {
  Function* fun;

  NAFont* mathFont;
  WindowController* windowController;
};

Application* _app = NA_NULL;



Application* allocApplication(void) {
  Application* app = naAlloc(Application);

  app->fun = allocFunction();

  app->mathFont = NA_NULL;
  app->windowController = NA_NULL;

  return app;
}



void startupApplicationGUI(Application* app) {
  app->mathFont = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  app->windowController = allocWindowController();
}



void deallocApplication(Application* app) {
  deallocFunction(app->fun);
  deallocWindowController(app->windowController);
  naRelease(app->mathFont);
  
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



Function* getGlobalFunction() {
  return _app->fun;
}

NAFont* getGlobalMathFont() {
  return _app->mathFont;
}

void drawGlobalScene() {
  updateWindowControllerScene(_app->windowController);
}
