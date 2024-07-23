#include <stdio.h>

#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"
#include "NAMath/NAVectorAlgebra.h"



#define PARAMS_COUNT 4


typedef struct Controller Controller;
struct Controller {
  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  NASlider* sliders[PARAMS_COUNT];
  
  double params[PARAMS_COUNT];
  double zoom;
};

void updateController(const Controller* con);



double evaluate(double t, const double* params) {
  return 10. * params[0] * sin(t) / t;
  //return params[0] * 10. * sin(t);
  //return params[0] * t*t*t + params[1] * t*t + params[2] * t + params[3];
}



void drawFunction(const Controller* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < viewRect.size.width; ++i) {
      double t = ((double)i - viewRect.size.width * .5) / con->zoom;
      glVertex2d(t, evaluate(t, con->params));
    }
  glEnd();
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
  rect.pos.x = 200.;
  rect.pos.y = 0.;
  rect.size.width -= 200;
  NARect oldRect = naGetUIElementRect(con->openGLSpace);
  if(!naEqualRect(oldRect, rect)) {
    naSetUIElementRect(con->openGLSpace, rect);
    naRefreshUIElement(con->openGLSpace, 0.);
  }
}



void paramChanged(NAReaction reaction) {
  Controller* con = reaction.controller; 
  
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    if(reaction.uiElement == con->sliders[i]) {
      con->params[i] = naGetSliderValue(con->sliders[i]);
      break;
    }
  }
  
  updateController(con);
}



void updateController(const Controller* con) {
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    naSetSliderValue(con->sliders[i], con->params[i]);
  }
  
  naRefreshUIElement(con->openGLSpace, 0.);
}



void postStartup(void* arg) {
  Controller* con = arg;
  
  NASize spaceSize = naMakeSize(600, 400);
  
  con->win = naNewWindow(
    "Plotty",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + 200, spaceSize.height)),
    NA_WINDOW_RESIZEABLE,
    0);
  naAddUIReaction(con->win, NA_UI_COMMAND_RESHAPE, reshapeWindow, con);

  // Add parameters
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    con->sliders[i] = naNewSlider(160);
    naAddUIReaction(con->sliders[i], NA_UI_COMMAND_EDITED, paramChanged, con);
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
    naAddSpaceChild(contentSpace, con->sliders[i], naMakePos(20, yOffset - i * 25));
  }
  naAddSpaceChild(contentSpace, con->openGLSpace, naMakePos(200, 0));

  // Put the window onscreen.
  naShowWindow(con->win);
}



void initController(Controller* con) {
  con->openGLSpace = NA_NULL;
  con->zoom = 15.;
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    con->params[i] = 0.;
  }
}



int main(int argc, const char * argv[]) {
  // Create global controller and initialize it.
  static Controller globalController;
  initController(&globalController);
  
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL, &globalController);
  naStopRuntime();
  return 0;
}
