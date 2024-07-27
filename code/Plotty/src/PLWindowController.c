
#include "PLWindowController.h"

#include "PLApplication.h"
#include "PLFunction.h"
#include "PLParam.h"
#include "PLParamController.h"

#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"



struct PLWindowController {
  double zoom;

  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  PLParamController** paramControllers;
};



void pl_DrawFunction(const PLWindowController* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  PLFunction* func = plGetGlobalFunction();
  size_t paramCount = plGetFunctionParamCount(func);

  double* params = naMalloc(sizeof(double) * paramCount);
  for(size_t i = 0; i < paramCount; ++i) {
    params[i] = plGetParamValue(plGetFunctionParameter(func, i));
  }
  
  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < viewRect.size.width; ++i) {
      double t = ((double)i - viewRect.size.width * .5) / con->zoom;
      glVertex2d(t, plEvaluateFunction(func, t, params));
    }
  glEnd();
  
  naFree(params);
}



void pl_repositionParameters(const PLWindowController* con) {
  PLFunction* func = plGetGlobalFunction();
  size_t paramCount = plGetFunctionParamCount(func);
  NASpace* contentSpace = naGetWindowContentSpace(con->win);

  double yOffset = naGetUIElementRect(contentSpace).size.height - MARGIN;
  for(size_t i = 0; i < paramCount; ++i) {
    NASpace* space = plGetParamControllerSpace(con->paramControllers[i]);
    NARect spaceRect = naGetUIElementRect(space);
    yOffset -= spaceRect.size.height;
    naAddSpaceChild(
      contentSpace,
      space,
      naMakePos(0, yOffset));
  }
}



void pl_reshapeWindow(NAReaction reaction) {
  const PLWindowController* con = reaction.controller; 

  NARect rect = naGetUIElementRect(con->win);
  rect.pos.x = SIDEBAR_WIDTH;
  rect.pos.y = 0.;
  rect.size.width -= SIDEBAR_WIDTH;
  NARect oldRect = naGetUIElementRect(con->openGLSpace);
  if(!naEqualRect(oldRect, rect)) {
    pl_repositionParameters(con);
    naSetUIElementRect(con->openGLSpace, rect);
    naRefreshUIElement(con->openGLSpace, 0.);
  }
}



void pl_drawScene(NAReaction reaction) {
  const PLWindowController* con = reaction.controller; 

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
  
  pl_DrawFunction(con);
  
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
}



void plUpdateWindowControllerScene(const PLWindowController* con) {
  naRefreshUIElement(con->openGLSpace, 0.);
}



void plUpdateWindowController(const PLWindowController* con) {
  PLFunction* func = plGetGlobalFunction();
  size_t paramCount = plGetFunctionParamCount(func);

  for(size_t i = 0; i < paramCount; ++i) {
    plUpdateParamController(con->paramControllers[i]);
  }
  
  plUpdateWindowControllerScene(con);
}



PLWindowController* plAllocWindowController(void) {
  PLWindowController* con = naAlloc(PLWindowController);

  con->zoom = 15.;

  NASize spaceSize = naMakeSize(600, 400);

  con->win = naNewWindow(
    "Paul",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + SIDEBAR_WIDTH, spaceSize.height)),
    NA_WINDOW_RESIZEABLE,
    0);
  naAddUIReaction(con->win, NA_UI_COMMAND_RESHAPE, pl_reshapeWindow, con);

  PLFunction* func = plGetGlobalFunction();
  size_t paramCount = plGetFunctionParamCount(func);
  con->paramControllers = naMalloc(sizeof(PLParamController*) * paramCount);

  // Add parameters
  for(size_t i = 0; i < paramCount; ++i) {
    con->paramControllers[i] = plAllocParamController(plGetFunctionParameter(func, i), i);
  }
  pl_repositionParameters(con);

  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);  
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, pl_drawScene, con);
  
  // Setup the UI.
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  
  naAddSpaceChild(
    contentSpace,
    con->openGLSpace,
    naMakePos(SIDEBAR_WIDTH, 0));

  // Put the window onscreen.
  naShowWindow(con->win);
  plUpdateWindowController(con);
  
  return con;
}



void plDeallocWindowController(PLWindowController* con) {
  PLFunction* func = plGetGlobalFunction();
  size_t paramCount = plGetFunctionParamCount(func);

  for(size_t i = 0; i < paramCount; ++i) {
    plDeallocParamController(con->paramControllers[i]);
  }
  naFree(con->paramControllers);
  
  naFree(con);
}
