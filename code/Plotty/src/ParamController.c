
#include "ParamController.h"

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
  
  for(size_t i = 0; i < PARAMS_COUNT; ++i) {
    if(reaction.uiElement == con->slider) {
      setParamValue(con->param, naGetSliderValue(con->slider));
      break;
    }else if(reaction.uiElement == con->textField) {
      setParamValue(con->param, naGetTextFieldDouble(con->textField));
      break;
    }
  }
  
  updateParamController(con);
  drawGlobalScene();
}



void paramButtonPressed(NAReaction reaction) {
  ParamController* con = reaction.controller; 
  if(reaction.uiElement == con->button) {
  
    ParamEditorController* paramCon = allocParamEditorController();
    setParamEditorControllerModel(paramCon, con->param);
    showParamEditorController(paramCon);
    
    // We wait till the modal window closes.
    
    deallocParamEditorController(paramCon);
    updateParamController(con);
  }
}



ParamController* allocParamController(Param* param, size_t index) {
  ParamController* con = naAlloc(ParamController);
  
  con->param = param;
  
  con->space = naNewSpace(naMakeSize(SIDEBAR_WIDTH, 2 * UI_ELEMENT_HEIGHT + 2 * VSPACER));
  naSetSpaceAlternateBackground(con->space, index % 2);
  
  con->label = naNewLabel(naAllocSprintf(NA_TRUE, "%c", 'a' + index), 20.);
//  naSetLabelFont(con->label, con->mathFont);
  con->textField = naNewTextField(TEXTFIELD_WIDTH);
  
  con->slider = naNewSlider(SIDEBAR_WIDTH - 2. * MARGIN);
  naSetSliderRange(
    con->slider,
    getParamMin(con->param),
    getParamMax(con->param),
    0);
  
  con->button = naNewTextPushButton("...", 30);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, paramChanged, con);
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, paramChanged, con);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, paramButtonPressed, con);

  naAddSpaceChild(
    con->space,
    con->label,
    naMakePos(MARGIN, VSPACER + 25));
  naAddSpaceChild(
    con->space,
    con->textField,
    naMakePos(MARGIN + 20., VSPACER + 25));
  naAddSpaceChild(
    con->space,
    con->button,
    naMakePos(SIDEBAR_WIDTH - MARGIN - 30., VSPACER + 25));
  naAddSpaceChild(
    con->space,
    con->slider,
    naMakePos(MARGIN, VSPACER + 0));

  return con;
}

void deallocParamController(ParamController* con) {
  // do nothing
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
