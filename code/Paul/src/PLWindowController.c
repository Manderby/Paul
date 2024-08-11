
#include "PLWindowController.h"

#include "PLApplication.h"
#include "PLFunction.h"
#include "PLParam.h"
#include "PLParamController.h"

#include "NAApp/NAApp.h"
#include "NAStruct/NAArray.h"
#include "NAVisual/NA3DHelper.h"


#define PL_SCROLL_SPEED 5.


struct PLWindowController {
  NAPos center;
  double zoom;
  size_t functionIndex;

  NAWindow* win;

  NASelect* functionSelect;
  NASpace* paramSpace;
  NAArray paramControllers;

  NAButton* zoomOutButton;
  NALabel* zoomLabel;
  NAButton* zoomInButton;
  
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

    NAArrayIterator iter = naMakeArrayMutator(&con->paramControllers);
    while(naIterateArray(&iter, 1)) {
      PLParamController** paramCon = naGetArrayCurMutable(&iter);
      NASpace* space = plGetParamControllerSpace(*paramCon);
      naRemoveSpaceChild(con->paramSpace, space);
      naDelete(space);
      plDeallocParamController(*paramCon);
    }
    naClearArrayIterator(&iter);
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

    
  if(plGetFunctionIntegerOnly(func)) {
    glPointSize(8);
    glBegin(GL_POINTS);
      glColor3ub(255, 255, 255);
      double left = con->center.x - viewRect.size.width * .5 / con->zoom;
      double right = con->center.x + viewRect.size.width * .5 / con->zoom;

      for(double x = left - 1.; x < right + 1.; x += 1.) {
        double t = naFloor(x);
        if(t >= plGetFunctionMinBound(func) && t <= plGetFunctionMaxBound(func)) {
          double result = plEvaluateFunction(func, t, params);
          glVertex2d(t, result);
        }
      }
    glEnd();
  }else{
    glBegin(GL_LINE_STRIP);
      glColor3ub(150, 150, 150);
      for(size_t i = 0; i < viewRect.size.width; ++i) {
        double t = con->center.x - viewRect.size.width * .5 / con->zoom;
        t += ((double)i / (double)viewRect.size.width) * viewRect.size.width / con->zoom;
        if(t >= plGetFunctionMinBound(func) && t <= plGetFunctionMaxBound(func)) {
          double result = plEvaluateFunction(func, t, params);
          glVertex2d(t, result);
        }
      }
    glEnd();
  }
  
  naFree(params);
}



void pl_recreateParameters(PLWindowController* con) {
  PLFunction* func = plGetFunction(con->functionIndex);
  size_t paramCount = plGetFunctionParamCount(func);

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

  naSetWindowTitle(con->win, plGetFunctionName(func));

  for(size_t i = 0; i < paramCount; ++i) {
    plUpdateParamController(*(PLParamController**)naGetArrayElementMutable(&con->paramControllers, i));
  }
  
  naSetLabelText(con->zoomLabel, naAllocSprintf(NA_TRUE, "%g", con->zoom));
  
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

  double left = con->center.x - viewRect.size.width * .5 / con->zoom;
  double right = con->center.x + viewRect.size.width * .5 / con->zoom;
  double bottom = con->center.y - viewRect.size.height * .5 / con->zoom;
  double top = con->center.y + viewRect.size.height * .5 / con->zoom;

  // Setup projection and modelview
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  NAMat44d ortho;
  naFillMatrixOrtho(
    ortho,
    left,
    right,
    bottom,
    top,
    -1, 1);
  glMultMatrixd(ortho);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear the scene with the background color.
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
    
  // Draw coordinate system
  glBegin(GL_LINES);
    glColor3ub(128, 128, 128);
    glVertex2d(left, 0.);
    glVertex2d(right, 0.);
    glVertex2d(0., bottom);
    glVertex2d(0., top);
  glEnd();

  glBegin(GL_QUADS);
    glColor3ub(32, 32, 32);
    glVertex2d(left, bottom);
    glVertex2d(left, top);
    glVertex2d(0., top);
    glVertex2d(0., bottom);
    glVertex2d(left, bottom);
    glVertex2d(left, 0.);
    glVertex2d(right, 0.);
    glVertex2d(right, bottom);
  glEnd();

  // Draw vertical integer lines
  glBegin(GL_LINES);
    for(size_t i = 0; i < 10000; i++) {
      if(i % 10){
        glColor3ub(64, 64, 64);
      }else
      {
        glColor3ub(128, 128, 128);
      }
      glVertex2d((double)i, bottom);
      glVertex2d((double)i, top);
    }

    for(size_t i = 0; i < 1000; i++) {
      if(i % 10){
        glColor3ub(64, 64, 64);
      }else
      {
        glColor3ub(196, 196, 196);
      }
      glVertex2d(left, (double)i);
      glVertex2d(right, (double)i);
    }
  glEnd();
  
  pl_DrawFunction(con);
  
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
}



void pl_panScene(NAReaction reaction) {
  PLWindowController* con = reaction.controller; 

  const NAMouseStatus* mouseStatus = naGetMouseStatus();
  if(naGetMouseButtonPressed(mouseStatus, NA_MOUSE_BUTTON_MIDDLE)) {
    NAPos delta = naGetMouseDelta(mouseStatus);
    con->center.x -= delta.x / con->zoom;
    con->center.y -= delta.y / con->zoom;
    plUpdateWindowControllerScene(con);
  }
}



void pl_transformScene(NAReaction reaction) {
  PLWindowController* con = reaction.controller; 
 
  NAKeyStroke keyStroke = naGetCurrentKeyStroke();
  NABool optionPressed  = naGetFlagu32(keyStroke.modifiers, NA_MODIFIER_FLAG_OPTION);
 
  const double* transformation = naGetOpenGLSpaceTransformation(con->openGLSpace);
  NAPos translation = naGetMat33dTranslation(transformation);
  double magnification = naGetMat33dMagnification(transformation);
 
  if(optionPressed) {
    const NAMouseStatus* mouseStatus = naGetMouseStatus();
    NAPos mousePos = naGetMousePos(mouseStatus);
    NARect sceneRect = naGetUIElementRectAbsolute(con->openGLSpace);

    NAPos mouseCoords = naMakePos(
      con->center.x - sceneRect.size.width * .5 / con->zoom + (mousePos.x - sceneRect.pos.x) / con->zoom,
      con->center.y - sceneRect.size.height * .5 / con->zoom + (mousePos.y - sceneRect.pos.y) / con->zoom);

    double zoomFactor = naPow(1.05, translation.y);

    con->center = naMakePos(
      con->center.x + (con->center.x - mouseCoords.x) * (1. - zoomFactor),
      con->center.y + (con->center.y - mouseCoords.y) * (1. - zoomFactor));

    con->zoom *= zoomFactor;
  }else{
    con->center.x -= PL_SCROLL_SPEED * translation.x / con->zoom;
    con->center.y -= PL_SCROLL_SPEED * translation.y / con->zoom;
  }
  
  plUpdateWindowControllerScene(con);
}



void pl_zoomScene(NAReaction reaction) {
  PLWindowController* con = reaction.controller; 

  if(reaction.uiElement == con->zoomInButton) {
    con->zoom *= 2.;
  } else if(reaction.uiElement == con->zoomOutButton) {
    con->zoom /= 2.;
  }
  
  pl_UpdateWindowController(con);
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

  con->center = naMakePos(19., 0.);
  con->zoom = 50.;
  con->functionIndex = 0;

  naInitArray(&con->paramControllers);

  NASize spaceSize = naMakeSize(1200, 600);

  con->win = naNewWindow(
    "Paul",
    naMakeRect(
      naMakePos(100, 200),
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

  con->zoomOutButton = naNewTextPushButton("-", SMALL_BUTTON_WIDTH);
  con->zoomLabel = naNewLabel("", BUTTON_WIDTH);
  naSetLabelTextAlignment(con->zoomLabel, NA_TEXT_ALIGNMENT_CENTER);
  con->zoomInButton = naNewTextPushButton("+", SMALL_BUTTON_WIDTH);
  naAddUIReaction(con->zoomOutButton, NA_UI_COMMAND_PRESSED, pl_zoomScene, con);
  naAddUIReaction(con->zoomInButton, NA_UI_COMMAND_PRESSED, pl_zoomScene, con);

  con->quitButton = naNewTextPushButton("Quit", BUTTON_WIDTH);
  naAddUIReaction(con->quitButton, NA_UI_COMMAND_PRESSED, pl_exitApplication, con);

  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);

  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, pl_drawScene, con);
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_TRANSFORMED, pl_transformScene, con);
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_MOUSE_MOVED, pl_panScene, con);
    
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
    con->zoomOutButton,
    naMakePos(MARGIN, MARGIN + 2 * UI_ELEMENT_HEIGHT));
  naAddSpaceChild(
    contentSpace,
    con->zoomLabel,
    naMakePos(MARGIN + SMALL_BUTTON_WIDTH + HSPACER, MARGIN + 2 * UI_ELEMENT_HEIGHT));
  naAddSpaceChild(
    contentSpace,
    con->zoomInButton,
    naMakePos(MARGIN + SMALL_BUTTON_WIDTH + BUTTON_WIDTH + 2 * HSPACER, MARGIN + 2 * UI_ELEMENT_HEIGHT));

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
