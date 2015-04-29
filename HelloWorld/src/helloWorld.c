//*****************************************************************************
// Luke Hsiao
// 29 April 2015
// Simple functions for printing to LCD and drawing to LCD
//*****************************************************************************

#include <stdio.h>
#include "supportFiles/display.h"

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
  
  // Screen size values
  uint16_t x_max = display_width();
  uint16_t y_max = display_height();
  
  // Values of screen positions
	uint16_t x_fourth = x_max/4;
	uint16_t x_half = x_max/2;
	uint16_t x_three_fourths = (display_width() * 3) / 4;
	uint16_t y_fourth = y_max/4;
	uint16_t y_half = y_max/2;
	uint16_t y_three_fourths = (y_max * 3) / 4;
  
  // Values for sizing the shapes
  uint16_t radius = x_max / 6;
  uint16_t triangle_dist_from_center = 24;
  
  display_setRotation(1);             // Sets origin to bottom left of screen
  display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
  
  // Draw the X that segments the screen
  display_drawLine( 0,  // original x-coordinate
                    0,  // original y-coordinate
                    x_max,  // destination x-coordinate
                    y_max,  // destination y-coordinate
                    DISPLAY_GREEN);
  
  display_drawLine( 0,  // original x-coordinate
                    y_max,  // original y-coordinate
                    x_max,  // destination x-coordinate
                    0,  // destination y-coordinate      
                    DISPLAY_GREEN);             

  // Draw the filled triangle
  display_fillTriangle( x_half - triangle_dist_from_center, // x0
                        y_fourth - triangle_dist_from_center, // y0
                        x_half + triangle_dist_from_center, // x1
                        y_fourth - triangle_dist_from_center, // y1
                        x_half, // x2
                        y_fourth + triangle_dist_from_center, // y2
                        DISPLAY_YELLOW);
  
  // Draw the triangle outline
  display_drawTriangle( x_half - triangle_dist_from_center,
                        y_three_fourths + triangle_dist_from_center,
                        x_half + triangle_dist_from_center,
                        y_three_fourths + triangle_dist_from_center,
                        x_half,
                        y_three_fourths - triangle_dist_from_center,
                        DISPLAY_YELLOW);
  
  // Draw the filled circle
  display_fillCircle( x_half, // x0
                      y_three_fourths, // y0
                      radius,  // radius
                      DISPLAY_RED);
  
  // Draw the circle outline
  display_drawCircle( x_half,
                      y_fourth,
                      radius,
                      DISPLAY_RED);  
}

int main() {
  display_init();  // init all of the software and underlying hardware for LCD.
  //printHelloWorld();  // Print "hello world (on the LCD)!"
  drawShapes();  // Draw 4 shapes and an X on the screen
  return 0;
}
