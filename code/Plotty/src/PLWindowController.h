
typedef struct PLWindowController PLWindowController;


// Alloc and dealloc window controller
PLWindowController* plAllocWindowController(void);
void plDeallocWindowController(PLWindowController* con);

// Update the different parts of the window
void plUpdateWindowControllerScene(const PLWindowController* con);
