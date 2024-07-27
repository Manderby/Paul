
#include "ParamController.h"

#include "Application.h"
#include "Param.h"
#include "ParamEditorController.h"

#include "NAApp/NAApp.h"

struct ParamController {
  Param* param;

  NASpace* space;
  NALabel* label;
  NATextField* textField;
  NASlider* slider;
  NAButton* button;
};

void udpateParamController(ParamController* con);



void paramChanged(NAReaction reaction) {
  ParamController* con = reaction.controller; 
  
  if(reaction.uiElement == con->slider) {
    setParamValue(con->param, naGetSliderValue(con->slider));
  }else if(reaction.uiElement == con->textField) {
    setParamValue(con->param, naGetTextFieldDouble(con->textField));
  }
  
  updateParamController(con);
  drawGlobalScene();
}



void paramButtonPressed(NAReaction reaction) {
  ParamController* con = reaction.controller; 
  if(reaction.uiElement == con->button) {
  
    ParamEditorController* paramCon = allocParamEditorController();
    setParamEditorControllerModel(paramCon, con->param);
    showParamEditorController(paramCon, naGetUIElementWindow(con->space));
    
    // We wait till the modal window closes.
    
    deallocParamEditorController(paramCon);
    updateParamController(con);
  }
}



ParamController* allocParamController(Param* param, size_t index) {
  ParamController* con = naAlloc(ParamController);
  
  con->param = param;
  
  con->space = naNewSpace(naMakeSize(SIDEBAR_WIDTH, UI_ELEMENT_HEIGHT));
  
  con->label = naNewLabel(naAllocSprintf(NA_TRUE, "%c", 'a' + index), PARAM_NAME_WIDTH);
  naSetLabelFont(con->label, getGlobalMathFont());
  con->textField = naNewTextField(TEXTFIELD_WIDTH);
  naSetTextFieldTextAlignment(con->textField, NA_TEXT_ALIGNMENT_RIGHT);
  
  double sliderWidth = SIDEBAR_WIDTH - 2 * MARGIN - PARAM_NAME_WIDTH - TEXTFIELD_WIDTH - PREF_BUTTON_WIDTH - 2 * HSPACER;
  
  con->slider = naNewSlider(sliderWidth);
  naSetSliderRange(
    con->slider,
    getParamMin(con->param),
    getParamMax(con->param),
    0);
  
  con->button = naNewTextPushButton("...", PREF_BUTTON_WIDTH);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, paramChanged, con);
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, paramChanged, con);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, paramButtonPressed, con);

  naAddSpaceChild(
    con->space,
    con->label,
    naMakePos(MARGIN, 0));
  naAddSpaceChild(
    con->space,
    con->textField,
    naMakePos(MARGIN + PARAM_NAME_WIDTH, 0));
  naAddSpaceChild(
    con->space,
    con->slider,
    naMakePos(MARGIN + PARAM_NAME_WIDTH + TEXTFIELD_WIDTH + 1 * HSPACER, 0));
  naAddSpaceChild(
    con->space,
    con->button,
    naMakePos(MARGIN + PARAM_NAME_WIDTH + TEXTFIELD_WIDTH + sliderWidth + 2 * HSPACER, 0));

  return con;
}

void deallocParamController(ParamController* con) {
  naFree(con);
}



void updateParamController(ParamController* con) {
  naSetTextFieldText(con->textField, naAllocSprintf(NA_TRUE, "%f", getParamValue(con->param)));
  naSetSliderRange(
    con->slider,
    getParamMin(con->param),
    getParamMax(con->param),
    0);
  naSetSliderValue(con->slider, getParamValue(con->param));
}



NASpace* getParamControllerSpace(ParamController* con){
  return con->space;
}
