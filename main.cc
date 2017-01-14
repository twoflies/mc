// main

#include "main.h"

#include <iostream>

int main(int argc, char **argv) {
  MasterControl masterControl;
  MasterControlConsole console(&masterControl);
  return console.mainLoop();
}
