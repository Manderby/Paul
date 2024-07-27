
typedef struct WindowController WindowController;



WindowController* allocWindowController(void);
void deallocWindowController(WindowController* con);

void updateWindowController(const WindowController* con);
void updateWindowControllerScene(const WindowController* con);
