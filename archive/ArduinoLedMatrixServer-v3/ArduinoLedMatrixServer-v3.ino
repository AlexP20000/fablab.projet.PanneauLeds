/*------------------------------------------------------------------------------

Connects to a WiFi network and displays the IP:PORT it's lestening to.
When connected to that IP with a Web browser, a from is displayed that lets
the user to display the text of his choice on the display.
Some info stuff is printed on the serial too.

TODO:
- Test if it works on Chrome and Safari
- Scrolling text: basic version done in code, no interface yet
- RTC and it's web interface elements

------------------------------------------------------------------------------*/

//wifi libs
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

//neomatrix libs
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h> //3x4 font

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
#define BRIGHTNESS 128  //overall brightness
//The font height, because custon fonts are printed starting from their baseline.
#define FONT_HEIGHT 4 //4 for picopixel font //for default font should be 0
#define FONT_WIDTH  3 //3 for picopixel font //for default font should be 6
#define SCROLL_DELAY_MS 200 //scrolling speed

// Some color definitions for NeoMatrix library
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE   0xFC00

// Global Wifi declaration
// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
SoftwareSerial uart(10, 3); //Pin 10 for TX becaue it's an Arduino Mega
WiFly wifly(&uart);
char ourip[22]; //For storing the IP:PORT we're listenig to. 21 is max lenght of IP:PORT
#define BUFFLEN 100
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
char matrixtext[100];

// RTC declaration
RTC_DS1307 rtc;

void setup() {
  serialInit();
  wifiInit();
  matrixInit();
  wifiPrintIp(ourip);
       

  //For testing
  //matrix.setFont();
  //matrix.setTextWrap(false); 
}

void loop() { 
  //wifiSerialRepeater(); //For testing.
  serverLoop();

  //Testing
  matrixScrollV(matrixtext);
}

void wifiInit() {
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

void wifiGetIP(char *ipout) { //Get IP:PORT on which our WiFi shield is listening.
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

void wifiPrintIp(char *ip) { //Get and print our ip:port to serial and display.
  wifiGetIP(ip);          //What's our IP:PORT?
  Serial.println(ip);     //Print it to serial.
  strcpy(matrixtext, ip); //Copy it in out display buffer.
  matrix.print(ip);       //Print it to display.
  matrix.show();          //Update the display.
}

void wifiSerialRepeater() { //Send serial to wifi and wifi to serial.
  while (wifly.available()) {
    Serial.write(wifly.read());
  }
  while (Serial.available()) {
    wifly.write(Serial.read());
  }
}

void matrixInit() {
  matrix.begin();
  matrix.clear();
  matrix.setBrightness(BRIGHTNESS);
  matrix.setFont(&Picopixel);
  matrix.setTextColor(RED);
  matrix.setCursor(0, FONT_HEIGHT);
  matrix.show();
}

void matrixClearPrint(char data[]) {//Clears the display and prints
  matrix.clear();
  matrix.setCursor(0, FONT_HEIGHT);
  matrix.print(data);
  matrix.show();
}

int scrolli = 0;
unsigned long scrolllastms =  millis();
void matrixScroll(char text[]) {
  if (scrolllastms + SCROLL_DELAY_MS < millis()) { //if a certain time passed
    int i = strlen(text)*FONT_WIDTH;
    if ( scrolli < -i ) {
      scrolli = LED_X;
    }
    scrolllastms = millis();
    matrix.clear();
    matrix.setCursor(scrolli, FONT_HEIGHT);
    matrix.print(text);
    matrix.show();
    scrolli--;
  }
}

void matrixScrollV(char text[]) { //vertical scrolling

  // The calculation: We want to find the amount of vertical pixels the text takes.
  // Amount of horizontal pixels the text would take: strlen(text) * FONT_WIDTH 
  // Divide by the actual amount of horizontal pixels: (strlen(text) * FONT_WIDTH) / LED_X
  // This gives us the lines required to display the text. Except it starts counting lines from 0 so:
  // (strlen(text) * FONT_WIDTH) / LED_X) + 1
  // Multiply by font height to get vertical pixels: (((strlen(text) * FONT_WIDTH) / LED_X) + 1) * FONT_HEIGHT

  // There is a problem with long texts not scrolling all the way through before restarting from the bottom.
  // Let's account for the spaces between charaters: (((strlen(text) * FONT_WIDTH + 1) / LED_X) + 1) * FONT_HEIGHT + 1
  // Didn't fix the problem. So for now we're more generous with the numbers
  
  if (scrolllastms + SCROLL_DELAY_MS < millis()) { //if a certain time passed
    int i = ((((strlen(text) * FONT_WIDTH + 3) / LED_X) + 3) * FONT_HEIGHT + 2); 
    if ( scrolli < -i ) {
      scrolli = LED_Y;
    }
    scrolllastms = millis();
    matrix.clear();
    matrix.setCursor(0, FONT_HEIGHT + scrolli);
    matrix.print(text);
    matrix.show();
    scrolli--;
  }
  
}

void serialInit() {
  Serial.begin(57600);
  Serial.println(F("INIT"));
}

void serial2Matrix() {
  while (Serial.available()) {
    matrix.print(Serial.read());
  }
}

int isItGet(char buff[]) { //Does the given char[] start with "GET /"?
  if (buff[0]=='G' && buff[1]=='E' && buff[2]=='T' && buff[3]==' ' && buff[4]=='/') {
    return 1;
  } else {
    return -1;
  }
}

void serveRootPage() {
  // HTTP header.
  wifly.print(F("HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n"));
  // HTML header. The "shortcut icon" is so browsers don't make million requests for a favicon.
  wifly.print(F("<!DOCTYPE html><title>LED Panel Control Page</title><link rel='icon' href='data:;base64,iVBORw0KGgo='><meta name='viewport' content='initial-scale=1,width=device-width'>"));
  // HTML content.
  wifly.print(F("<p>Text to display <small>(50 chars max)</small></p><form method='get'><input type='text' name='t' maxlength='50'><input type='submit' value='Send text'></form><br>"));
  wifly.print(F("<p>Color</p><form method='get'><select name='c'>"));
  wifly.print(F("<option value='r'>Red</option><option value='b'>Blue</option><option value='g'>Green</option><option value='w'>White</option>"));
  wifly.print(F("<option value='c'>Cyan</option><option value='m'>Magenta</option><option value='y'>Yellow</option><option value='o'>Orange</option>"));
  wifly.print(F("</select><input type='submit' value='Set color'></form><br>"));
  
  //wifly.print(F("<p>Set time<pre>(YYYY-MM-DD HH:MM)</pre></p><form method='get'><input type='text' name='d'><input type='submit' value='Set the clock'></form>"));
  //wifly.print(F("<p>Date when this page was sent: "));
  //DateTime now = rtc.now();
  //wifly.print(now .year());
}

void getTextRequest(char buff[]) {//Decode a GET request into its text.
  //Only works if there is 1 command in the GET request.
  //Isolate the text of the request (what comes after "/?t=")
  strcpy(buff, strtok(buff, " "));
  strcpy(buff, strtok(NULL, " "));
  strcpy(buff, strtok(buff, "="));
  strcpy(buff, strtok(NULL, "="));
  //Deal with the + and % codes
  decodeWebString(buff);
}

void serverLoop() { //To be put in loop()
  if(wifly.available()) {
    int i;
    delay(1000);
    for (i=0; i<BUFFLEN; i++) {
      buff[i] = wifly.read();
    }
    wifly.clear();

    Serial.println(buff); //For testing

    //Interpret requests
    if (isItGet(buff) && buff[5]==' ') { //Does the request begin with "GET / "?
      serveRootPage();
      delay(500);
    } else if (isItGet(buff) && buff[5]=='?' && buff[6]=='t') {
      // "GET /?t" is the text command
      serveRootPage();
      getTextRequest(buff);
      strcpy(matrixtext, buff); //So that we can reprint old text with new colors etc.
      matrixClearPrint(matrixtext);
      Serial.println(buff);
      delay(500);
    } else if (isItGet(buff) && buff[5]=='?' && buff[6]=='c') {
      // Color command  
      serveRootPage();
      switch(buff[8]) {
        case 'r':
        matrix.setTextColor(RED);
        break;
        
        case 'b':
        matrix.setTextColor(BLUE);
        break;
        
        case 'g':
        matrix.setTextColor(GREEN);
        break;
        
        case 'c':
        matrix.setTextColor(CYAN);
        break;
        
        case 'm':
        matrix.setTextColor(MAGENTA);
        break;
        
        case 'w':
        matrix.setTextColor(WHITE);
        break;
        
        case 'y':
        matrix.setTextColor(YELLOW);
        break;

        case 'o':
        matrix.setTextColor(ORANGE);
        break;
      }
      matrixClearPrint(matrixtext);
    } else if (isItGet(buff) && buff[5]=='?' && buff[6]=='d') {
      // "GET /?d" is the clock set command
      serveRootPage();
      getTextRequest(buff);
      Serial.print(F("Time Input: "));
      Serial.println(buff);
    } else {
      wifly.print(F("HTTP/1.1 404 Not Found\nContent-Type: text/html\nConnection: close\n\n404"));
    }
  }
}

void decodeWebString(char data[]) {//Decode the text in GET requests. data should be null terminated.
  //Replace each + with a space first
  int i;
  for (i=0; data[i]!='\0'; i++) {
    if (data[i] == '+') {
      data[i] = ' ';
    }
  }
  
  //Now decode the % codes
  //From https://arduino.stackexchange.com/a/18008
  
  // Create two pointers that point to the start of the data
  char *leader = data;
  char *follower = leader;

  // While we're not at the end of the string (current character not NULL)
  while (*leader) {
    // Check to see if the current character is a %
    if (*leader == '%') {

        // Grab the next two characters and move leader forwards
        leader++;
        char high = *leader;
        leader++;
        char low = *leader;

        // Convert ASCII 0-9A-F to a value 0-15
        if (high > 0x39) high -= 7;
        high &= 0x0f;

        // Same again for the low byte:
        if (low > 0x39) low -= 7;
        low &= 0x0f;

        // Combine the two into a single byte and store in follower:
        *follower = (high << 4) | low;
    } else {
        // All other characters copy verbatim
        *follower = *leader;
    }

    // Move both pointers to the next character:
    leader++;
    follower++;
  }
  // Terminate the new string with a NULL character to trim it off
  *follower = 0;
}

void decodeTimeRequest(char data[]) {
  
}
