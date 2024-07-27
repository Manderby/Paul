#include "main.h"

#include "NAApp/NAApp.h"
#include "Application.h"



int main(int argc, const char * argv[]) {
  naStartRuntime();
  naStartApplication(startupApplication, startupGUI, shutdownApplication, NA_NULL);
  naStopRuntime();
  return 0;
}
