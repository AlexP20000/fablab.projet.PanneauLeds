/*--------

Connects to a WiFi network and displays the IP:PORT it's lestening to.
When connected to that IP with a Web browser, a from is displayed that lets
the user to display the text of his choice on the display.
Some info stuff is printed on the serial too.

TODO:
- Scrolling text
- RTC and it's web interface elements
- Decoding the URL percentage codes in the received text

--------*/

//wifi libs
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

//neomatrix libs
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>

//wifi login
#define SSID      "YOUR_WIFI_SSID"
#define KEY       "YOUR_WIFI_PASSWORD"
#define AUTH      WIFLY_AUTH_WPA2_PSK

// LED NeoMatrix definitions
#define LED_PIN    6   //which pin is the led data line on
#define LED_X      30  //led amount in the x axis
#define LED_Y      25  //led amount in the y axis
#define LED_COUNT  750 //total amount of leds
#define BRIGHTNESS 64  //overall brightness
//The font height, because custon fonts are printed starting from their baseline.
#define FONT_HEIGHT 4

// Some color definitions for NeoMatrix library
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

// Global Wifi declaration
// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
SoftwareSerial uart(10, 3); //Pin 10 for TX becaue it's an Arduino Mega
WiFly wifly(&uart);
char ourip[22]; //For storing the IP:PORT we're listenig to. 21 is max lenght of IP:PORT
#define BUFFLEN 64
char buff[BUFFLEN];  //Buffer for received requests.

// Global NeoMatrix declaration
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
// Text we're displaying
char matrixtext[64];

// RTC declaration
RTC_DS1307 rtc;

void setup() {
  SerialInit();
  WifiInit();
  MatrixInit();
  WifiPrintIP(ourip);
       

  //For testing
  //matrix.setTextWrap(false);
}

void loop() { 
  //WifiSerialRepeater(); //For testing.
  ServerLoop();
}

void WifiInit() {
  uart.begin(57600);
  
  Serial.println("Join " SSID);
  if (wifly.join(SSID, KEY, AUTH)) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("Failed"));
  }

  wifly.sendCommand("set ip proto 18\r");   // set HTML mode
  wifly.sendCommand("set ip local 80\r");   // set the local comm port to 80
  wifly.sendCommand("set comm remote 0\r"); // do not send a default string when a connection opens
  wifly.sendCommand("set comm open 0\r");   // set the string that the wifi shield will output when a connection is opened
  wifly.sendCommand("set comm close 0\r");  // same for closing connections
  // close the tcp connection after 1 second, otherwise web browsers will never finish loading the page we send them
  wifly.sendCommand("set comm idle 1\r");

}

void WifiGetIP(char *ipout) { //Get IP:PORT on which our WiFi shield is listening.
  delay(5000);
  wifly.sendCommand("get ip\r");
  char c;
  char ip[22];
  //search for "IP=", if found, take 22 chars beyond that (max lenght of IP:port+\0)
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) {
    if ((char)c == 'I') {
      wifly.receive((uint8_t *)&c, 1, 300);
      if ((char)c == 'P') {
        wifly.receive((uint8_t *)&c, 1, 300);
        if ((char)c == '=') {
          wifly.receive((uint8_t *)&ip, 22, 300);
        }
      }
    }
  }
  strcpy(ipout, strtok(ip, "\n"));
}

void WifiPrintIP(char *ip) { //Get and print our ip:port to serial and display.
  WifiGetIP(ip);      //What's our IP:PORT?
  Serial.println(ip); //Print it to serial.
  matrix.print(ip);   //Print it to display.
  matrix.show();      //Update the display.
}

void WifiSerialRepeater() { //Send serial to wifi and wifi to serial.
  while (wifly.available()) {
    Serial.write(wifly.read());
  }
  while (Serial.available()) {
    wifly.write(Serial.read());
  }
}

void MatrixInit() {
  matrix.begin();
  matrix.clear();
  matrix.setBrightness(BRIGHTNESS);
  matrix.setFont(&Picopixel);
  matrix.setTextColor(BLUE);
  matrix.setCursor(0, FONT_HEIGHT);
  matrix.show();
}

void SerialInit() {
  Serial.begin(57600);
  Serial.println(F("INIT"));
}

void Serial2Matrix() {
  while (Serial.available()) {
    matrix.print(Serial.read());
  }
}

int IsItGet(char buff[]) { //Does the given buffer start with "GET /"?
  if (buff[0]=='G' && buff[1]=='E' && buff[2]=='T' && buff[3]==' ' && buff[4]=='/') {
    return 1;
  } else {
    return -1;
  }
}

void ServeRootPage() {
  // HTTP header.
  wifly.print(F("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n"));
  // HTML header. The "shortcut icon" is so browsers don't make million requests for a favicon.
  wifly.print(F("<!DOCTYPE html><html><head><title>Panneau affichage LED</title><link rel='icon' href='data:;base64,iVBORw0KGgo='><meta name='viewport' content='initial-scale=1'>"));
  // HTML content.
  wifly.print(F("<body><h1>Text to display</h1><form method='get'><input type='text' name='t'><input type='submit' value='Send'>"));
}

void GetTextRequest(char buff[]) {
  int i;
  strcpy(buff, strtok(buff, " "));
  strcpy(buff, strtok(NULL, " "));
  strcpy(buff, strtok(buff, "="));
  strcpy(buff, strtok(NULL, "="));
  
  //Replace + with a space (URL percentage encoding thing)
  for (i=0; i<BUFFLEN; i++) {
    if (buff[i] == '+') {
      buff[i] = ' ';
    }
  }
  Serial.println(buff);
}

void ServerLoop() {
  if(wifly.available()) {
    int i;
    delay(1000);
    for (i=0; i<BUFFLEN; i++) {
      buff[i] = wifly.read();
    }
    wifly.clear();
    //wifly.read((char *)buff, BUFFLEN);

    //Serial.println(buff); //For testing

    if (IsItGet(buff) && buff[5]==' ') { //Does the request begin with "GET / "?
      ServeRootPage();
      delay(500);
    } else if (IsItGet(buff) && buff[5]=='?' && buff[6]=='t') {// "GET /&t" is the text command.
      ServeRootPage();
      GetTextRequest(buff);
      matrix.clear();
      matrix.setCursor(0, FONT_HEIGHT);
      matrix.print(buff);
      matrix.show();
      delay(500);
    }
  }
}
