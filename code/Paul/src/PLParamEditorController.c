
#include "PLParamEditorController.h"
#include "PLParam.h"

#include "NAApp/NAApp.h"



struct PLParamEditorController {
  PLParam* param;
  
  NAWindow* win;
  
  NALabel* minLabel;
  NALabel* maxLabel;
  NATextField* minTextField;
  NATextField* maxTextField;
  
  NAButton* button;
};



void pl_UdpateParamEditorController(PLParamEditorController* con) {
  naSetTextFieldText(con->minTextField, naAllocSprintf(NA_TRUE, "%f", plGetParamMin(con->param)));
  naSetTextFieldText(con->maxTextField, naAllocSprintf(NA_TRUE, "%f", plGetParamMax(con->param)));
}



void pl_EditProperty(NAReaction reaction) {
  PLParamEditorController* con = reaction.controller;

  if(reaction.uiElement == con->minTextField) {
    plSetParamMin(con->param, naGetTextFieldDouble(con->minTextField));
  }else if(reaction.uiElement == con->maxTextField) {
    plSetParamMax(con->param, naGetTextFieldDouble(con->maxTextField));
  }
  
  pl_UdpateParamEditorController(con);
}



void pl_closeEditor(NAReaction reaction) {
  PLParamEditorController* con = reaction.controller;
  
  naCloseWindowModal(con->win);
}



PLParamEditorController* plAllocParamEditorController() {
  PLParamEditorController* con = naAlloc(PLParamEditorController);
  
  con->param = NA_NULL;
  
  con->win = naNewWindow("Edit Parameter", naMakeRectS(200, 200, SIDEBAR_WIDTH, 200), 0, 0);
  naAddUIReaction(con->win, NA_UI_COMMAND_CLOSES, pl_closeEditor, con);

  con->minLabel = naNewLabel("Min:", LABEL_WIDTH);
  con->maxLabel = naNewLabel("Max:", LABEL_WIDTH);

  con->minTextField = naNewTextField(TEXTFIELD_WIDTH);
  con->maxTextField = naNewTextField(TEXTFIELD_WIDTH);
  naAddUIReaction(con->minTextField, NA_UI_COMMAND_EDIT_FINISHED, pl_EditProperty, con);
  naAddUIReaction(con->maxTextField, NA_UI_COMMAND_EDIT_FINISHED, pl_EditProperty, con);
    
  con->button = naNewTextPushButton("Close", BUTTON_WIDTH);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, pl_closeEditor, con);
  
  NASpace* contentSpace = naGetWindowContentSpace(con->win);
  naAddSpaceChild(contentSpace, con->minLabel, naMakePos(MARGIN, 100));
  naAddSpaceChild(contentSpace, con->minTextField, naMakePos(MARGIN + LABEL_WIDTH + HSPACER, 100));
  naAddSpaceChild(contentSpace, con->maxLabel, naMakePos(MARGIN, 75));
  naAddSpaceChild(contentSpace, con->maxTextField, naMakePos(MARGIN + LABEL_WIDTH + HSPACER, 75));
  naAddSpaceChild(contentSpace, con->button, naMakePos(SIDEBAR_WIDTH * .5 - 50, MARGIN));
  
  naSetButtonSubmit(con->button, pl_closeEditor, con);

  return con;
}



void plDeallocParamEditorController(PLParamEditorController* con) {
  naFree(con);
}



void plShowParamEditorController(PLParamEditorController* con, NAWindow* parentWindow) {
  pl_UdpateParamEditorController(con);
  naShowWindowModal(con->win, parentWindow);
}



void plSetParamEditorControllerModel(PLParamEditorController* con, PLParam* param) {
  con->param = param;
  pl_UdpateParamEditorController(con);
}
