
#include "main.h"

PLOTTY_PROTOTYPE(Param);
PLOTTY_PROTOTYPE(NAWindow);



typedef struct ParamEditorController ParamEditorController;



ParamEditorController* allocParamEditorController(void);
void deallocParamEditorController(ParamEditorController* con);

void showParamEditorController(ParamEditorController* con, NAWindow* parentWindow);

void setParamEditorControllerModel(ParamEditorController* con, Param* param);
