#include <stdio.h>

#include "NAApp.h"
#include "NA3DHelper.h"
#include "NAVectorAlgebra.h"



typedef struct Controller Controller;
struct Controller {
  NAWindow* win;
  NAOpenGLSpace* openGLSpace;
  double zoom;
};



double evaluate(double t) {
  return 10. * sin(t);
//  return a * t^2 + b * t + c;
}



void drawFunction(const Controller* con) {
  NARect viewRect = naGetUIElementRect(con->openGLSpace);

  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < viewRect.size.width; ++i) {
      double t = ((double)i - viewRect.size.width * .5) / con->zoom;
      glVertex2d(t, evaluate(t));
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



void postStartup(void* arg) {
  Controller* con = arg;
  
  NASize spaceSize = naMakeSize(600, 400);
  
  con->win = naNewWindow(
    "Plotty",
    naMakeRect(
      naMakePos(100, 100),
      naMakeSize(spaceSize.width + 200, spaceSize.height)),
    0,
    0);
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  
  // Add the drawing region.
  con->openGLSpace = naNewOpenGLSpace(
    spaceSize,
    NA_NULL,
    NA_NULL);
  naAddSpaceChild(
    contentSpace,
    con->openGLSpace,
    naMakePos(200, 0));
  
  naAddUIReaction(
    con->openGLSpace,
    NA_UI_COMMAND_REDRAW,
    drawScene,
    con);
  
  // Put the window onscreen.
  naShowWindow(con->win);
}



int main(int argc, const char * argv[]) {
  // Create global controller and initialize it.
  static Controller globalController;
  globalController.openGLSpace = NA_NULL;
  globalController.zoom = 15.;
  
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL, &globalController);
  naStopRuntime();
  return 0;
}
