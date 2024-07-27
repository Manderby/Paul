
#include "main.h"

PL_PROTOTYPE(PLFunction);
PL_PROTOTYPE(NAFont);



// Starting and stopping the application.
void plStartupApplication(void* arg);
void plStartupGUI(void* arg);
void plShutdownApplication(void* arg);

// Function called by main
PLFunction* plGetGlobalFunction(void);
NAFont* plGetGlobalMathFont(void);
void plDrawGlobalScene(void);

// External functions:
void plUserStartup(void);
void plUserShutdown(void);
