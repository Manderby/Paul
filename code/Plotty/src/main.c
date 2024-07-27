#include <stdio.h>

#include "main.h"

#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"
#include "NAMath/NAVectorAlgebra.h"

#include "Param.h"
#include "ParamController.h"



typedef struct Controller Controller;
struct Controller {
  NAFont* mathFont;
  
  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  
  ParamController* paramControllers[PARAMS_COUNT];
  
  Param* params[PARAMS_COUNT];
  double zoom;
};

void updateController(const Controller* con);
void updateScene(const Controller* con);



double evaluate(double t, const double* p) {
  //return 10. * params[0] * sin(t) / t;
  //return params[0] * 10. * sin(t);
  return p[0] * t*t*t + p[1] * t*t + p[2] * t + p[3];
}



void drawFunction(const Controller* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  double* params = naMalloc(sizeof(double) * PARAMS_COUNT);
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    params[i] = getParamValue(con->params[i]);
  }
  
  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < viewRect.size.width; ++i) {
      double t = ((double)i - viewRect.size.width * .5) / con->zoom;
      glVertex2d(t, evaluate(t, params));
    }
  glEnd();
  
  naFree(params);
}



void drawScene(NAReaction reaction) {
  const Controller* con = reaction.controller; 

  NARect viewRect = naGetUIElementRect(con->openGLSpace);
  glViewport(
    (GLint)0,
    (GLint)0,
    (GLsizei)viewRect.size.width,
    (GLsizei)viewRect.size.height);

  // Setup projection and modelview
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  NAMat44d ortho;
  naFillMatrixOrtho(
    ortho,
    -viewRect.size.width * .5 / con->zoom,
    +viewRect.size.width * .5 / con->zoom,
    -viewRect.size.height * .5 / con->zoom,
    +viewRect.size.height * .5 / con->zoom,
    -1, 1);
  glMultMatrixd(ortho);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear the scene with the background color.
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
  
  // Draw coordinate system
  glBegin(GL_LINES);
    glVertex2d(-100., 0.);
    glVertex2d(+100., 0.);
    glVertex2d(0., -100.);
    glVertex2d(0., +100.);
  glEnd();
  
  drawFunction(con);
  
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
}



void reshapeWindow(NAReaction reaction) {
  const Controller* con = reaction.controller; 

  NARect rect = naGetUIElementRect(con->win);
  rect.pos.x = SIDEBAR_WIDTH;
  rect.pos.y = 0.;
  rect.size.width -= SIDEBAR_WIDTH;
  NARect oldRect = naGetUIElementRect(con->openGLSpace);
  if(!naEqualRect(oldRect, rect)) {
    naSetUIElementRect(con->openGLSpace, rect);
    naRefreshUIElement(con->openGLSpace, 0.);
  }
}



void updateController(const Controller* con) {
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    updateParamController(con->paramControllers[i]);
  }
  updateScene(con);
}

void updateScene(const Controller* con) {
  naRefreshUIElement(con->openGLSpace, 0.);
}



void postStartup(void* arg) {
  Controller* con = arg;
  
  NASize spaceSize = naMakeSize(600, 400);
  
  con->mathFont = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  
  con->win = naNewWindow(
    "Plotty",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + SIDEBAR_WIDTH, spaceSize.height)),
    NA_WINDOW_RESIZEABLE,
    0);
  naAddUIReaction(con->win, NA_UI_COMMAND_RESHAPE, reshapeWindow, con);

  // Add parameters
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    con->paramControllers[i] = allocParamController(con->params[i], i);
  }

  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);  
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, drawScene, con);
  
  // Setup the UI.
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  double yOffset = 200.;
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    NASpace* space = getParamControllerSpace(con->paramControllers[i]);
    NARect spaceRect = naGetUIElementRect(space);
    naAddSpaceChild(
      contentSpace,
      space,
      naMakePos(0, (PARAMS_COUNT - 1) * spaceRect.size.height - i * spaceRect.size.height));
  }
  naAddSpaceChild(
    contentSpace,
    con->openGLSpace,
    naMakePos(SIDEBAR_WIDTH, 0));

  // Put the window onscreen.
  naShowWindow(con->win);
  updateController(con);
}



void initController(Controller* con) {
  con->openGLSpace = NA_NULL;
  con->zoom = 15.;
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    con->params[i] = allocParam();
  }
}



void cleanup(void* arg) {
  Controller* con = arg;

  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    deallocParam(con->params[i]);
  }

  naRelease(con->mathFont);
}



Controller globalController;

int main(int argc, const char * argv[]) {
  // Create global controller and initialize it.
  //static Controller globalController;
  initController(&globalController);
  
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, cleanup, &globalController);
  naStopRuntime();
  return 0;
}



Controller* getGlobalController() {
  return &globalController;
}

void drawGlobalScene(void) {
  updateScene(&globalController);
}
