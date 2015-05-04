//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Driver file for testing buttons and switches
//*****************************************************************************

#include "buttons.h"
#include "switches.h"

int main() {
  // Run Button Test
  buttons_runTest();

  // Run Switches Test after Button Test completes
  switches_runTest();

  return 0;
}
