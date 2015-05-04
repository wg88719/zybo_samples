//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Driver file for testing buttons and switches
//*****************************************************************************

#include "buttons.h"
#include "switches.h"
#include "supportFiles/leds.h"
#include "supportFiles/display.h"

int main() {
  // Initialize all of the used hardware components
  buttons_init();  // Initialize buttons
  switches_init();  // Initialize switches

  // Run Button Test
  buttons_runTest();

  // Run Switches Test after Button Test completes
  switches_runTest();

  return 0;
}
