//Draws a "snake" with each pixel having a random color.
//Intended for testing.

#include <Adafruit_NeoPixel.h>

#define LED_COUNT  750 //total amount of leds
#define LED_PIN    6   //which pin is the led data line on
#define BRIGHTNESS 64  //overall brightness 0-255

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN);

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
}
    
void loop() {
  strip.clear();
  strip.show();
  for (int i = 0; i < LED_COUNT; i++) {
    //strip.setPixelColor(i, random(0x7FFFFFFF));
    strip.setPixelColor(i, strip.Color(
				       random(0,255), random(0,255),
				       random(0,255)));
    //strip.setPixelColor(i, strip.Color(127, 127, 127));
    strip.show();
    delay(1);
  }
}
