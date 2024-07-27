
#include "main.h"

PLOTTY_PROTOTYPE(Function);
PLOTTY_PROTOTYPE(Param);
PLOTTY_PROTOTYPE(NAFont);

typedef struct Application Application;



void startupApplication(void* arg);
void startupGUI(void* arg);
void shutdownApplication(void* arg);

Function* getGlobalFunction(void);
NAFont* getGlobalMathFont(void);
void drawGlobalScene(void);
