
#include "main.h"

PLOTTY_PROTOTYPE(Param);
PLOTTY_PROTOTYPE(NAFont);

typedef struct Application Application;



void startupApplication(void* arg);
void startupGUI(void* arg);
void shutdownApplication(void* arg);

NAFont* getGlobalMathFont(void);
Param* getGlobalParam(size_t index);
void drawGlobalScene(void);
