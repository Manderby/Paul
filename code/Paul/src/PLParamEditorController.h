
#include "main.h"

PL_PROTOTYPE(PLParam);
PL_PROTOTYPE(NAWindow);



typedef struct PLParamEditorController PLParamEditorController;



// Alloc and dealloc the controller
PLParamEditorController* plAllocParamEditorController(void);
void plDeallocParamEditorController(PLParamEditorController* con);

// Show the controller
void plShowParamEditorController(PLParamEditorController* con, NAWindow* parentWindow);

// Set the model of the controller
void plSetParamEditorControllerModel(PLParamEditorController* con, PLParam* param);
