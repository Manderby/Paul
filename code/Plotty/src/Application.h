
#include "main.h"

PLOTTY_PROTOTYPE(Param);

typedef struct Application Application;



void startupApplication(void* arg);
void startupGUI(void* arg);
void shutdownApplication(void* arg);

Param* getGlobalParam(size_t index);
void drawGlobalScene(void);
