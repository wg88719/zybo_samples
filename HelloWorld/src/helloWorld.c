//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Simple functions for printing to LCD and drawing to LCD
//*****************************************************************************

#include <stdio.h>
#include "supportFiles/display.h"

void printHelloWorld() {
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
  display_setCursor(0, 0);            // The upper left of the LCD screen.
  display_setTextColor(DISPLAY_RED);  // Make the text red.
  display_setTextSize(2);             // Make the text a little larger.
  display_println("hello world (on the LCD)!");    // This actually prints the string to the LCD.
  printf("hello world!\n\r");  // This prints on the console.
}

void drawShapes() {
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
  
  
}

int main() {
  display_init();  // Must init all of the software and underlying hardware for LCD.
  printHelloWorld();  // Print "hello world (on the LCD)!"

}
