
#include "main.h"

PLOTTY_PROTOTYPE(Param);
PLOTTY_PROTOTYPE(NASpace);



typedef struct ParamController ParamController;



ParamController* allocParamController(Param* param, size_t index);
void deallocParamController(ParamController* con);

void updateParamController(ParamController* con);
NASpace* getParamControllerSpace(ParamController* con);
