//Switches between different fonts represented by different colors.
//Intended to find a suitable font.

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

// Fonts
#include <Fonts/TomThumb.h>
#include <Fonts/Org_01.h>
#include <Fonts/Picopixel.h>
#include <Fonts/Tiny3x3a2pt7b.h>

#define LED_PIN    6   //which pin is the led data line on
#define LED_X      30  //led amount in the x axis
#define LED_Y      25  //led amount in the y axis
#define LED_COUNT  750 //total amount of leds
#define BRIGHTNESS 64  //overall brightness

// Some color definitions for NeoMatrix library
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(LED_X, LED_Y, LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
    
void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  matrix.clear();
  matrix.setBrightness(BRIGHTNESS);
  matrix.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  matrix.clear();
  matrix.setFont(&TomThumb);
  matrix.setTextColor(RED);
  matrix.setCursor(0, 5);
  matrix.print("Hello 1234567890");
  matrix.show();
  delay(3000);

  matrix.clear();
  matrix.setFont(&Org_01);
  matrix.setTextColor(GREEN);
  matrix.setCursor(0, 5);
  matrix.print("Hello 1234567890");
  matrix.show();
  delay(3000);

  matrix.clear();
  matrix.setFont(&Picopixel);
  matrix.setTextColor(BLUE);
  matrix.setCursor(0, 5);
  matrix.print("Hello 1234567890");
  matrix.show();
  delay(3000);
  
  matrix.clear();
  matrix.setFont(&Tiny3x3a2pt7b);
  matrix.setTextColor(MAGENTA);
  matrix.setCursor(0, 3);
  matrix.print("Hello 1234567890");
  matrix.show();
  delay(3000);

  matrix.clear();
  matrix.setFont();
  matrix.setTextColor(WHITE);
  matrix.setCursor(0, 0);
  matrix.print("Hello 1234567890");
  matrix.show();
  delay(3000);
}
