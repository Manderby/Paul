
#include "PLApplication.h"

#include "PLFunction.h"
#include "PLParam.h"
#include "PLWindowController.h"

#include "NAApp/NAApp.h"



typedef struct PLApplication PLApplication;
struct PLApplication {
  PLFunction* func;

  NAFont* mathFont;
  PLWindowController* windowController;
};



// The global application object.
PLApplication* _app = NA_NULL;



PLApplication* plAllocApplication(void) {
  PLApplication* app = naAlloc(PLApplication);

  app->func = plAllocFunction();
  
  app->mathFont = NA_NULL;
  app->windowController = NA_NULL;

  return app;
}



void plStartupApplicationGUI(PLApplication* app) {
  app->mathFont = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  app->windowController = plAllocWindowController();
}



void plDeallocApplication(PLApplication* app) {
  plDeallocWindowController(app->windowController);
  naRelease(app->mathFont);
  
  plDeallocFunction(app->func);
  
  naFree(app);
}



PLFunction* plGetGlobalFunction() {
  return _app->func;
}

NAFont* plGetGlobalMathFont() {
  return _app->mathFont;
}

void plDrawGlobalScene() {
  plUpdateWindowControllerScene(_app->windowController);
}



void plStartupApplication(void* arg) {
  _app = plAllocApplication();
}



void plStartupGUI(void* arg) {
  plUserStartup();
  plStartupApplicationGUI(_app);
}



void plShutdownApplication(void* arg) {
  plUserShutdown();
  plDeallocApplication(_app);
}
