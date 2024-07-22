#include <stdio.h>

#include "NAApp.h"
#include "NA3DHelper.h"
#include "NAVectorAlgebra.h"



NAOpenGLSpace* openGLSpace;
double zoom = 20.;


void drawFunction() {
  glBegin(GL_LINE_STRIP);
    for(size_t i = 0; i < 100; ++i) {
      double t = (double)i / 10. - 5.;
      glVertex2d(t, 5. * sin(t));
    }
  glEnd();
}



void drawScene(NAReaction reaction) {
  NARect viewRect = naGetUIElementRect(openGLSpace);
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
    -viewRect.size.width * .5 / zoom,
    +viewRect.size.width * .5 / zoom,
    -viewRect.size.height * .5 / zoom,
    +viewRect.size.height * .5 / zoom,
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
  
  drawFunction();
  
  naSwapOpenGLSpaceBuffer(openGLSpace);
}



void postStartup(void* arg) {
  NASize spaceSize = naMakeSize(600, 400);
  NAWindow* win = naNewWindow("Plotty", naMakeRect(naMakePos(100, 100), spaceSize), 0, 0);
  NASpace* contentSpace = naGetWindowContentSpace(win);
  
  openGLSpace = naNewOpenGLSpace(spaceSize, NA_NULL, NA_NULL);
  naAddSpaceChild(contentSpace, openGLSpace, naMakePos(0, 0));
  
  naAddUIReaction(openGLSpace, NA_UI_COMMAND_REDRAW, drawScene, NA_NULL);
  
  naShowWindow(win);
}



int main(int argc, const char * argv[]) {
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL, NA_NULL);
  naStopRuntime();
  return 0;
}
