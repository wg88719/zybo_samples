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
	leds_init(1);  // Initialize the LEDs
	buttons_init();
	switches_init();
	display_init();

	buttons_runTest();
	switches_runTest();
	return 0;
}
