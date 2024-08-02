
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
  
  con->label = naNewLabel(naAllocSprintf(NA_TRUE, "%c", 'a' + index), PARAM_NAME_WIDTH);
  naSetLabelFont(con->label, plGetGlobalMathFont());
  con->textField = naNewTextField(TEXTFIELD_WIDTH);
  naSetTextFieldTextAlignment(con->textField, NA_TEXT_ALIGNMENT_RIGHT);
  
  double sliderWidth = SIDEBAR_WIDTH - 2 * MARGIN - PARAM_NAME_WIDTH - TEXTFIELD_WIDTH - PREF_BUTTON_WIDTH - 2 * HSPACER;
  
  con->slider = naNewSlider(sliderWidth);
  naSetSliderRange(
    con->slider,
    plGetParamMin(con->param),
    plGetParamMax(con->param),
    0);
  
  con->button = naNewTextPushButton("...", PREF_BUTTON_WIDTH);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, pl_ParamChanged, con);
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, pl_ParamChanged, con);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, pl_ParamButtonPressed, con);

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
