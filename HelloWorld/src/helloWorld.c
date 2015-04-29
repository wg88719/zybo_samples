//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Simple functions for printing to LCD and drawing to LCD
//*****************************************************************************

#include <stdio.h>
#include "supportFiles/display.h"

#define X_MAX 23
#define Y_MAX 15

// Blanks the LCD screen and then prints "hellow world (on the LCD)!" to the LCD
void printHelloWorld() {
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
  display_setCursor(0, 0);            // The upper left of the LCD screen.
  display_setTextColor(DISPLAY_RED);  // Make the text red.
  display_setTextSize(2);             // Make the text a little larger.
  display_println("hello world (on the LCD)!");    // This actually prints the string to the LCD.
  printf("hello world!\n\r");  // This prints on the console.
}

// Draws an X to divide the screen into sections, and draws 4 shapes:
// 1. Filled triangle
// 2. Filled circle
// 3. Circle outline
// 4. Triangle outline
void drawShapes() {
  display_setRotation(1);             // Sets origin to bottom left of screen
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
  
  // Draw the X that segments the screen
  display_drawLine( 0,  // original x-coordinate
                    0,  // original y-coordinate
                    X_MAX,  // destination x-coordinate
                    Y_MAX,  // destination y-coordinate
                    DISPLAY_GREEN);
  
  display_drawLine( 0,  // original x-coordinate
                    Y_MAX,  // original y-coordinate
                    X_MAX,  // destination x-coordinate
                    0,  // destination y-coordinate      
                    DISPLAY_GREEN);             

  // Draw the filled triangle
  display_fillTriangle( x0, // x0
                        y0, // y0
                        x1, // x1
                        y1, // y1
                        x2, // x2
                        y2, // y2
                        DISPLAY_YELLOW);
  
  // Draw the triangle outline
  display_drawTriangle( x0,
                        y0,
                        x1,
                        y1,
                        x2,
                        y2,
                        DISPLAY_YELLOW);
  
  // Draw the filled circle
  display_fillCircle( x0, // x0
                      y0, // y0
                      r,  // radius
                      DISPLAY_RED);
  
  // Draw the circle outline
  display_drawCircle( x0,
                      y0,
                      r,
                      DISPLAY_RED);  
}

int main() {
  display_init();  // init all of the software and underlying hardware for LCD.
  //printHelloWorld();  // Print "hello world (on the LCD)!"
  drawShapes();  // Draw 4 shapes and an X on the screen
  return 0;
}
