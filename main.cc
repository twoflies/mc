// main

#include "main.h"

#include <iostream>

int main(int argc, char** argv) {
  MasterControl masterControl;
  int result = masterControl.initialize();
  if (result != OK) {
    return result;
  }
  
  MasterControlConsole console(&masterControl);
  result = console.initialize();
  if (result != OK) {
    return result;
  }
  
  result = console.mainLoop();

  console.destroy();
  masterControl.destroy();
}
