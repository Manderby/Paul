#include "main.h"

#include "NAApp/NAApp.h"
#include "PLApplication.h"



int main(int argc, const char * argv[]) {
  naStartRuntime();
  
  naStartApplication(
    plStartupApplication,
    plStartupGUI,
    plShutdownApplication,
    NA_NULL);
  
  naStopRuntime();
  
  return 0;
}
