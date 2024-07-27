
#include "main.h"

PL_PROTOTYPE(PLParam);
PL_PROTOTYPE(NASpace);



typedef struct PLParamController PLParamController;



// Alloc and dealloc the controller
PLParamController* plAllocParamController(PLParam* param, size_t index);
void plDeallocParamController(PLParamController* con);

// Update the controller
void plUpdateParamController(PLParamController* con);

// Returns the space the controller UI elements reside in.
NASpace* plGetParamControllerSpace(PLParamController* con);
