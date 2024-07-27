
#include "main.h"

PLOTTY_PROTOTYPE(Param);



typedef struct ParamEditorController ParamEditorController;



ParamEditorController* allocParamEditorController(void);
void deallocParamEditorController(ParamEditorController* con);

void showParamEditorController(ParamEditorController* con);

void setParamEditorControllerModel(ParamEditorController* con, Param* param);
