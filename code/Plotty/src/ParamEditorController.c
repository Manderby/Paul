
#include "ParamEditorController.h"
#include "Param.h"

#include "NAApp/NAApp.h"

struct ParamEditorController {
  Param* param;
  
  NAWindow* win;
  
  NALabel* minLabel;
  NALabel* maxLabel;
  NATextField* minTextField;
  NATextField* maxTextField;
  
  NAButton* button;
};

void udpateParamEditorController(ParamEditorController* con);



void editProperty(NAReaction reaction) {
  ParamEditorController* con = reaction.controller;

  if(reaction.uiElement == con->minTextField) {
    setParamMin(con->param, naGetTextFieldDouble(con->minTextField));
  }else if(reaction.uiElement == con->maxTextField) {
    setParamMax(con->param, naGetTextFieldDouble(con->maxTextField));
  }
  
  udpateParamEditorController(con);
}


void closeEditor(NAReaction reaction) {
  ParamEditorController* con = reaction.controller;
  
  naCloseWindowModal(con->win);
}



ParamEditorController* allocParamEditorController() {
  ParamEditorController* con = naAlloc(ParamEditorController);
  
  con->param = NA_NULL;
  
  con->win = naNewWindow("Edit Parameter", naMakeRectS(200, 200, SIDEBAR_WIDTH, 200), 0, 0);
  naAddUIReaction(con->win, NA_UI_COMMAND_CLOSES, closeEditor, con);

  con->minLabel = naNewLabel("Min:", LABEL_WIDTH);
  con->maxLabel = naNewLabel("Max:", LABEL_WIDTH);

  con->minTextField = naNewTextField(TEXTFIELD_WIDTH);
  con->maxTextField = naNewTextField(TEXTFIELD_WIDTH);
  naAddUIReaction(con->minTextField, NA_UI_COMMAND_EDIT_FINISHED, editProperty, con);
  naAddUIReaction(con->minTextField, NA_UI_COMMAND_EDIT_FINISHED, editProperty, con);
    
  con->button = naNewTextPushButton("Close", 100);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, closeEditor, con);
  
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  naAddSpaceChild(contentSpace, con->minLabel, naMakePos(MARGIN, 100));
  naAddSpaceChild(contentSpace, con->minTextField, naMakePos(MARGIN + LABEL_WIDTH + HSPACER, 100));
  naAddSpaceChild(contentSpace, con->maxLabel, naMakePos(MARGIN, 75));
  naAddSpaceChild(contentSpace, con->maxTextField, naMakePos(MARGIN + LABEL_WIDTH + HSPACER, 75));
  naAddSpaceChild(contentSpace, con->button, naMakePos(SIDEBAR_WIDTH * .5 - 50, MARGIN));
  
  naSetButtonSubmit(con->button, closeEditor, con);

  return con;
}

void deallocParamEditorController(ParamEditorController* con) {
  naFree(con);
}

void showParamEditorController(ParamEditorController* con, NAWindow* parentWindow) {
  udpateParamEditorController(con);
  naShowWindowModal(con->win, parentWindow);
}



void setParamEditorControllerModel(ParamEditorController* con, Param* param) {
  con->param = param;
  udpateParamEditorController(con);
}

void udpateParamEditorController(ParamEditorController* con) {
  naSetTextFieldText(con->minTextField, naAllocSprintf(NA_TRUE, "%f", getParamMin(con->param)));
  naSetTextFieldText(con->maxTextField, naAllocSprintf(NA_TRUE, "%f", getParamMax(con->param)));
}
