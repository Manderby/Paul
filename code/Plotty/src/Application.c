
#include "Application.h"

#include "Param.h"
#include "WindowController.h"

#include "NAApp/NAApp.h"



struct Application {
  Param* params[PARAMS_COUNT];

  NAFont* mathFont;
  WindowController* windowController;
};

Application* _app = NA_NULL;



Application* allocApplication(void) {
  Application* app = naAlloc(Application);
  
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    app->params[i] = allocParam();
  }

  app->mathFont = NA_NULL;
  app->windowController = NA_NULL;

  return app;
}

void startupApplicationGUI(Application* app) {
  app->mathFont = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  app->windowController = allocWindowController();
}

void deallocApplication(Application* app) {
  deallocWindowController(app->windowController);
  naRelease(app->mathFont);

  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    deallocParam(app->params[i]);
  }
}



void startupApplication(void* arg) {
  _app = allocApplication();
}

void startupGUI(void* arg) {
  startupApplicationGUI(_app);
}

void shutdownApplication(void* arg) {
  deallocApplication(_app);
}



Param* getGlobalParam(size_t index) {
  return _app->params[index];
}

void drawGlobalScene() {
  updateWindowControllerScene(_app->windowController);
}
