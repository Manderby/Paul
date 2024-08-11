
#include "PLApplication.h"

#include "PLFunction.h"
#include "PLParam.h"
#include "PLWindowController.h"

#include "NAApp/NAApp.h"
#include "NAStruct/NAList.h"



typedef struct PLApplication PLApplication;
struct PLApplication {
  PLFunction* func;
  NAList funcs;

  NAFont* mathFont;
  PLWindowController* windowController;
};



// The global application object.
PLApplication* _app = NA_NULL;



PLApplication* plAllocApplication(void) {
  PLApplication* app = naAlloc(PLApplication);

  app->func = plAllocFunction();
  
  naInitList(&app->funcs);
  
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
  
  naForeachListMutable(&app->funcs, (NAMutator)plDeallocFunction);
  naClearList(&app->funcs);
  
  plDeallocFunction(app->func);
  
  naFree(app);
}



size_t plGetFunctionCount() {
  return naGetListCount(&_app->funcs);
}



PLFunction* plGetFunction(size_t index) {
  PLFunction* func = NA_NULL;
  NAListIterator iter = naMakeListMutator(&_app->funcs);
  size_t i = 0;
  while(naIterateList(&iter)) {
    if(i == index) {
      func = naGetListCurMutable(&iter);
      break;;
    }
    i++;
  }
  naClearListIterator(&iter);
  return func;
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
  naSetDefaultWindowSystemKeyHandling(NA_FALSE);
}



void plShutdownApplication(void* arg) {
  plUserShutdown();
  plDeallocApplication(_app);
}



void plRegisterFunction(PLFunction* func) {
  naAddListLastMutable(&_app->funcs, func);
}
