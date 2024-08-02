
#include "main.h"

PL_PROTOTYPE(PLFunction);
PL_PROTOTYPE(NAFont);



// Starting and stopping the application.
void plStartupApplication(void* arg);
void plStartupGUI(void* arg);
void plShutdownApplication(void* arg);

void plRegisterFunction(PLFunction* func);

size_t plGetFunctionCount();
PLFunction* plGetFunction(size_t index);

// Function called by main
NAFont* plGetGlobalMathFont(void);
void plDrawGlobalScene(void);

// External functions:
void plUserStartup(void);
void plUserShutdown(void);
