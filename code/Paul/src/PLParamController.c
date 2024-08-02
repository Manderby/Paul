
#include "PLParamController.h"

#include "PLApplication.h"
#include "PLParam.h"
#include "PLParamEditorController.h"

#include "NAApp/NAApp.h"



struct PLParamController {
  PLParam* param;

  NASpace* space;
  NALabel* label;
  NATextField* textField;
  NASlider* slider;
  NAButton* button;
};



void pl_ParamChanged(NAReaction reaction) {
  PLParamController* con = reaction.controller; 
  
  if(reaction.uiElement == con->slider) {
    plSetParamValue(con->param, naGetSliderValue(con->slider));
  }else if(reaction.uiElement == con->textField) {
    plSetParamValue(con->param, naGetTextFieldDouble(con->textField));
  }
  
  plUpdateParamController(con);
  plDrawGlobalScene();
}



void pl_ParamButtonPressed(NAReaction reaction) {
  PLParamController* con = reaction.controller; 
  if(reaction.uiElement == con->button) {
  
    PLParamEditorController* paramCon = plAllocParamEditorController();
    plSetParamEditorControllerModel(paramCon, con->param);
    plShowParamEditorController(paramCon, naGetUIElementWindow(con->space));
    
    // We wait till the modal window closes.
    
    plDeallocParamEditorController(paramCon);
    plUpdateParamController(con);
  }
}



PLParamController* plAllocParamController(PLParam* param, size_t index) {
  PLParamController* con = naAlloc(PLParamController);
  
  con->param = param;
  
  con->space = naNewSpace(naMakeSize(SIDEBAR_WIDTH, UI_ELEMENT_HEIGHT));
  
  const NAUTF8Char* paramName = plGetParamName(param);
  NAUTF8Char* displayName = paramName
    ? naAllocSprintf(NA_TRUE, "%s", paramName)
    : naAllocSprintf(NA_TRUE, "p%d", (int)index);
  con->label = naNewLabel(displayName, PARAM_NAME_WIDTH);
  naSetLabelTextAlignment(con->label, NA_TEXT_ALIGNMENT_RIGHT);
  naSetLabelFont(con->label, plGetGlobalMathFont());
  naSetLabelHeight(con->label, 20);
  
  con->textField = naNewTextField(TEXTFIELD_WIDTH);
  naSetTextFieldTextAlignment(con->textField, NA_TEXT_ALIGNMENT_RIGHT);
  
  double sliderWidth = SIDEBAR_WIDTH - 2 * MARGIN - PARAM_NAME_WIDTH - HSPACER - TEXTFIELD_WIDTH - SMALL_BUTTON_WIDTH - 2 * HSPACER;
  
  con->slider = naNewSlider(sliderWidth);
  naSetSliderRange(
    con->slider,
    plGetParamMin(con->param),
    plGetParamMax(con->param),
    0);
  
  con->button = naNewTextPushButton("...", SMALL_BUTTON_WIDTH);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, pl_ParamChanged, con);
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, pl_ParamChanged, con);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, pl_ParamButtonPressed, con);

  naAddSpaceChild(
    con->space,
    con->label,
    naMakePos(MARGIN - PARAM_NAME_OVERSHOOT, -2));
  naAddSpaceChild(
    con->space,
    con->textField,
    naMakePos(MARGIN + PARAM_NAME_WIDTH - PARAM_NAME_OVERSHOOT + HSPACER, 0));
  naAddSpaceChild(
    con->space,
    con->slider,
    naMakePos(MARGIN + PARAM_NAME_WIDTH - PARAM_NAME_OVERSHOOT + HSPACER + TEXTFIELD_WIDTH + 1 * HSPACER, 0));
  naAddSpaceChild(
    con->space,
    con->button,
    naMakePos(MARGIN + PARAM_NAME_WIDTH - PARAM_NAME_OVERSHOOT + HSPACER + TEXTFIELD_WIDTH + sliderWidth + 2 * HSPACER, 0));

  return con;
}



void plDeallocParamController(PLParamController* con) {
  naFree(con);
}



void plUpdateParamController(PLParamController* con) {
  naSetTextFieldText(con->textField, naAllocSprintf(NA_TRUE, "%f", plGetParamValue(con->param)));
  naSetSliderRange(
    con->slider,
    plGetParamMin(con->param),
    plGetParamMax(con->param),
    0);
  naSetSliderValue(con->slider, plGetParamValue(con->param));
}



NASpace* plGetParamControllerSpace(PLParamController* con){
  return con->space;
}
