
#include "WindowController.h"

#include "Application.h"
#include "Function.h"
#include "Param.h"
#include "ParamController.h"

#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"



struct WindowController {
  double zoom;

  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  ParamController** paramControllers;
};



void drawFunction(const WindowController* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  Function* fun = getGlobalFunction();
  size_t paramCount = getFunctionParamCount(fun);

  double* params = naMalloc(sizeof(double) * paramCount);
  for(size_t i = 0; i < paramCount; ++i) {
    params[i] = getParamValue(getFunctionParameter(fun, i));
  }
  
  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < viewRect.size.width; ++i) {
      double t = ((double)i - viewRect.size.width * .5) / con->zoom;
      glVertex2d(t, evaluateFunction(fun, t, params));
    }
  glEnd();
  
  naFree(params);
}



void repositionParameters(const WindowController* con) {
  Function* fun = getGlobalFunction();
  size_t paramCount = getFunctionParamCount(fun);
  NASpace* contentSpace = naGetWindowContentSpace(con->win);

  double yOffset = naGetUIElementRect(contentSpace).size.height - MARGIN;
  for(size_t i = 0; i < paramCount; ++i) {
    NASpace* space = getParamControllerSpace(con->paramControllers[i]);
    NARect spaceRect = naGetUIElementRect(space);
    yOffset -= spaceRect.size.height;
    naAddSpaceChild(
      contentSpace,
      space,
      naMakePos(0, yOffset));
  }
}

void reshapeWindow(NAReaction reaction) {
  const WindowController* con = reaction.controller; 

  NARect rect = naGetUIElementRect(con->win);
  rect.pos.x = SIDEBAR_WIDTH;
  rect.pos.y = 0.;
  rect.size.width -= SIDEBAR_WIDTH;
  NARect oldRect = naGetUIElementRect(con->openGLSpace);
  if(!naEqualRect(oldRect, rect)) {
    repositionParameters(con);
    naSetUIElementRect(con->openGLSpace, rect);
    naRefreshUIElement(con->openGLSpace, 0.);
  }
}



void drawScene(NAReaction reaction) {
  const WindowController* con = reaction.controller; 

  double uiScale = naGetUIElementResolutionScale(con->win);
  NARect viewRect = naGetUIElementRect(con->openGLSpace);
  glViewport(
    (GLint)0,
    (GLint)0,
    (GLsizei)(viewRect.size.width * uiScale),
    (GLsizei)(viewRect.size.height * uiScale));

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



void updateWindowControllerScene(const WindowController* con) {
  naRefreshUIElement(con->openGLSpace, 0.);
}



void updateWindowController(const WindowController* con) {
  Function* fun = getGlobalFunction();
  size_t paramCount = getFunctionParamCount(fun);

  for(size_t i = 0; i < paramCount; ++i) {
    updateParamController(con->paramControllers[i]);
  }
  
  updateWindowControllerScene(con);
}



WindowController* allocWindowController(void) {
  WindowController* con = naAlloc(WindowController);

  con->zoom = 15.;

  NASize spaceSize = naMakeSize(600, 400);

  con->win = naNewWindow(
    "Plotty",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + SIDEBAR_WIDTH, spaceSize.height)),
    NA_WINDOW_RESIZEABLE,
    0);
  naAddUIReaction(con->win, NA_UI_COMMAND_RESHAPE, reshapeWindow, con);

  Function* fun = getGlobalFunction();
  size_t paramCount = getFunctionParamCount(fun);
  con->paramControllers = naMalloc(sizeof(ParamController*) * paramCount);

  // Add parameters
  for(size_t i = 0; i < paramCount; ++i) {
    con->paramControllers[i] = allocParamController(getFunctionParameter(fun, i), i);
  }
  repositionParameters(con);

  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);  
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, drawScene, con);
  
  // Setup the UI.
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  
  naAddSpaceChild(
    contentSpace,
    con->openGLSpace,
    naMakePos(SIDEBAR_WIDTH, 0));

  // Put the window onscreen.
  naShowWindow(con->win);
  updateWindowController(con);
  
  return con;
}



void deallocWindowController(WindowController* con) {
  Function* fun = getGlobalFunction();
  size_t paramCount = getFunctionParamCount(fun);

  for(size_t i = 0; i < paramCount; ++i) {
    deallocParamController(con->paramControllers[i]);
  }
  naFree(con->paramControllers);
  
  naFree(con);
}
