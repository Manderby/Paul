
#include "PLWindowController.h"

#include "PLApplication.h"
#include "PLFunction.h"
#include "PLParam.h"
#include "PLParamController.h"

#include "NAApp/NAApp.h"
#include "NAStruct/NAArray.h"
#include "NAVisual/NA3DHelper.h"



struct PLWindowController {
  double zoom;
  size_t functionIndex;

  NAWindow* win;
  NASelect* functionSelect;
  NASpace* paramSpace;
  NAArray paramControllers;
  NAButton* quitButton;
  NAOpenGLSpace* openGLSpace;
};



void pl_FreeParamControllers(PLWindowController* con);
void pl_DrawFunction(const PLWindowController* con);
void pl_recreateParameters(PLWindowController* con);
void pl_repositionUIElements(PLWindowController* con, NABool recreate);
void pl_UpdateWindowController(PLWindowController* con);
void pl_changeFunction(NAReaction reaction);
void pl_reshapeWindow(NAReaction reaction);
void pl_drawScene(NAReaction reaction);
void pl_exitApplication(NAReaction reaction);



void pl_FreeParamControllers(PLWindowController* con) {
  if(naGetArrayCount(&con->paramControllers)) {
    naForeachArraypMutable(&con->paramControllers, (NAMutator)plDeallocParamController);
    naClearArray(&con->paramControllers);
  }
}



void pl_DrawFunction(const PLWindowController* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  PLFunction* func = plGetFunction(con->functionIndex);
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



void pl_recreateParameters(PLWindowController* con) {
  PLFunction* func = plGetFunction(con->functionIndex);
  size_t paramCount = plGetFunctionParamCount(func);

  naRemoveAllSpaceChilds(con->paramSpace);
  pl_FreeParamControllers(con);

  // Add parameters
  naInitArrayWithCount(
    &con->paramControllers,
    sizeof(PLParamController*),
    paramCount);

  double yOffset = 0;
  for(size_t i = 0; i < paramCount; ++i) {
    PLParamController** paramController = naGetArrayElementMutable(&con->paramControllers, i);
    *paramController = plAllocParamController(plGetFunctionParameter(func, i), i);

    NASpace* space = plGetParamControllerSpace(*paramController);
    NARect spaceRect = naGetUIElementRect(space);
    yOffset -= spaceRect.size.height;
    naAddSpaceChild(
      con->paramSpace,
      space,
      naMakePos(0, yOffset));
  }
  
  NARect spaceRect = naGetUIElementRect(con->paramSpace);
  naShiftSpaceChilds(con->paramSpace, naMakePos(0, -yOffset));
  spaceRect.size.height = -yOffset;
  naSetUIElementRect(con->paramSpace, spaceRect);
}



void pl_repositionUIElements(PLWindowController* con, NABool recreate) {
  NARect contentRect = naGetUIElementRect(naGetWindowContentSpace(con->win));

  if(recreate) { pl_recreateParameters(con); }
  
  NARect selectRect = naGetUIElementRect(con->functionSelect);
  selectRect.pos.y = contentRect.size.height - MARGIN - UI_ELEMENT_HEIGHT;
  naSetUIElementRect(con->functionSelect, selectRect);

  NARect paramRect = naGetUIElementRect(con->paramSpace);
  paramRect.pos.y = selectRect.pos.y - UI_ELEMENT_HEIGHT - paramRect.size.height;
  naSetUIElementRect(con->paramSpace, paramRect);
}


void pl_UpdateWindowController(PLWindowController* con) {
  PLFunction* func = plGetFunction(con->functionIndex);
  size_t paramCount = plGetFunctionParamCount(func);

  for(size_t i = 0; i < paramCount; ++i) {
    plUpdateParamController(*(PLParamController**)naGetArrayElementMutable(&con->paramControllers, i));
  }
  
  plUpdateWindowControllerScene(con);
}



void pl_changeFunction(NAReaction reaction) {
  PLWindowController* con = reaction.controller; 

  size_t index = naGetSelectItemIndex(con->functionSelect, reaction.uiElement);
  if(index < plGetFunctionCount()) {
    con->functionIndex = index;
    pl_repositionUIElements(con, /*recreate*/ NA_TRUE);
    pl_UpdateWindowController(con);
  }
}



void pl_reshapeWindow(NAReaction reaction) {
  PLWindowController* con = reaction.controller; 

  NARect rect = naGetUIElementRect(con->win);
  rect.pos.x = SIDEBAR_WIDTH;
  rect.pos.y = 0.;
  rect.size.width -= SIDEBAR_WIDTH;
  NARect oldRect = naGetUIElementRect(con->openGLSpace);
  if(!naEqualRect(oldRect, rect)) {
    pl_repositionUIElements(con, /*recreate*/ NA_FALSE);
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



void pl_exitApplication(NAReaction reaction) {
  NA_UNUSED(reaction);
  naStopApplication();
}



void plUpdateWindowControllerScene(const PLWindowController* con) {
  naRefreshUIElement(con->openGLSpace, 0.);
}



PLWindowController* plAllocWindowController(void) {
  PLWindowController* con = naAlloc(PLWindowController);

  con->zoom = 15.;
  con->functionIndex = 0;

  naInitArray(&con->paramControllers);

  NASize spaceSize = naMakeSize(600, 400);

  con->win = naNewWindow(
    "Paul",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + SIDEBAR_WIDTH, spaceSize.height)),
    NA_WINDOW_RESIZEABLE,
    0);
  naAddUIReaction(con->win, NA_UI_COMMAND_RESHAPE, pl_reshapeWindow, con);

  con->functionSelect = naNewSelect(SIDEBAR_WIDTH - 2 * MARGIN);
  for(size_t i = 0; i < plGetFunctionCount(); ++i) {
    const PLFunction* func = plGetFunction(i);
    NAMenuItem* item = naNewMenuItem(plGetFunctionName(func));
    naAddUIReaction(item, NA_UI_COMMAND_PRESSED, pl_changeFunction, con);
    naAddSelectMenuItem(con->functionSelect, item, NA_NULL);
  }

  con->paramSpace = naNewSpace(naMakeSize(SIDEBAR_WIDTH, 500));
//  naSetSpaceAlternateBackground(con->paramSpace, NA_TRUE);

  con->quitButton = naNewTextPushButton("Quit", BUTTON_WIDTH);
  naAddUIReaction(con->quitButton, NA_UI_COMMAND_PRESSED, pl_exitApplication, con);

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
    con->functionSelect,
    naMakePos(MARGIN, 0));

  naAddSpaceChild(
    contentSpace,
    con->paramSpace,
    naMakePos(0, 0));

  pl_repositionUIElements(con, /*recreate*/ NA_TRUE);

  naAddSpaceChild(
    contentSpace,
    con->quitButton,
    naMakePos((SIDEBAR_WIDTH - BUTTON_WIDTH) * .5, MARGIN));

  naAddSpaceChild(
    contentSpace,
    con->openGLSpace,
    naMakePos(SIDEBAR_WIDTH, 0));

  // Put the window onscreen.
  naShowWindow(con->win);
  pl_UpdateWindowController(con);
  
  return con;
}



void plDeallocWindowController(PLWindowController* con) {
  PLFunction* func = plGetFunction(con->functionIndex);
  size_t paramCount = plGetFunctionParamCount(func);

  pl_FreeParamControllers(con);
  
  naFree(con);
}
