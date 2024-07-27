#include <stdio.h>

#include "main.h"

#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"
#include "NAMath/NAVectorAlgebra.h"

#include "Param.h"
#include "ParamEditorController.h"



#define PARAMS_COUNT 4



typedef struct Controller Controller;
struct Controller {
  NAFont* mathFont;
  
  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  
  struct {
    NALabel* label;
    NATextField* textField;
    NASlider* slider;
    NAButton* button;
  } inputs[PARAMS_COUNT];
  
  Param* params[PARAMS_COUNT];
  double zoom;
};

void updateController(const Controller* con);



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



void paramChanged(NAReaction reaction) {
  Controller* con = reaction.controller; 
  
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    if(reaction.uiElement == con->inputs[i].slider) {
      setParamValue(con->params[i], naGetSliderValue(con->inputs[i].slider));
      break;
    }else if(reaction.uiElement == con->inputs[i].textField) {
      setParamValue(con->params[i], naGetTextFieldDouble(con->inputs[i].textField));
      break;
    }
  }
  
  updateController(con);
}



void paramButtonPressed(NAReaction reaction) {
  Controller* con = reaction.controller; 
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    if(reaction.uiElement == con->inputs[i].button) {
    
      ParamEditorController* paramCon = allocParamEditorController();
      setParamEditorControllerModel(paramCon, con->params[i]);
      showParamEditorController(paramCon);
      
      // We wait till the modal window closes.
      
      deallocParamEditorController(paramCon);
      updateController(con);
    }
  }
}



void updateController(const Controller* con) {
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    naSetTextFieldText(con->inputs[i].textField, naAllocSprintf(NA_TRUE, "%f", getParamValue(con->params[i])));
    naSetSliderRange(
      con->inputs[i].slider,
      getParamMin(con->params[i]),
      getParamMax(con->params[i]),
      0);
    naSetSliderValue(con->inputs[i].slider, getParamValue(con->params[i]));
  }
  
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
    con->inputs[i].label = naNewLabel(naAllocSprintf(NA_TRUE, "%c", 'a' + i), 20.);
    naSetLabelFont(con->inputs[i].label, con->mathFont);
    con->inputs[i].textField = naNewTextField(TEXTFIELD_WIDTH);
    
    con->inputs[i].slider = naNewSlider(SIDEBAR_WIDTH - 2. * MARGIN);
    naSetSliderRange(
      con->inputs[i].slider,
      getParamMin(con->params[i]),
      getParamMax(con->params[i]),
      0);
    
    con->inputs[i].button = naNewTextPushButton("...", 30);
    naAddUIReaction(con->inputs[i].textField, NA_UI_COMMAND_EDIT_FINISHED, paramChanged, con);
    naAddUIReaction(con->inputs[i].slider, NA_UI_COMMAND_EDITED, paramChanged, con);
    naAddUIReaction(con->inputs[i].button, NA_UI_COMMAND_PRESSED, paramButtonPressed, con);
  }

  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);  
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, drawScene, con);
  
  // Setup the UI.
  double yOffset = 200;
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    naAddSpaceChild(
      contentSpace,
      con->inputs[i].label,
      naMakePos(MARGIN, yOffset - (2 * i + 0) * 25));
    naAddSpaceChild(
      contentSpace,
      con->inputs[i].textField,
      naMakePos(MARGIN + 20., yOffset - (2 * i + 0) * 25));
    naAddSpaceChild(
      contentSpace,
      con->inputs[i].button,
      naMakePos(SIDEBAR_WIDTH - MARGIN - 30., yOffset - (2 * i + 0) * 25));
    naAddSpaceChild(
      contentSpace,
      con->inputs[i].slider,
      naMakePos(MARGIN, yOffset - (2 * i + 1) * 25));
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



int main(int argc, const char * argv[]) {
  // Create global controller and initialize it.
  static Controller globalController;
  initController(&globalController);
  
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, cleanup, &globalController);
  naStopRuntime();
  return 0;
}
