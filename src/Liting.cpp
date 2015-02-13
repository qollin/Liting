/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE

  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel

  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look.

  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide

  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/
#include "WS2812_Definitions.h"
#include <RotaryEncoder.h>
#include "Adafruit_NeoPixel.h"
#include "Adafruit_MCP23017.h"
#include "TimerOne.h"
#include "MCP23017PinBank.h"

#define PIN 4
#define LED_COUNT 1
uint32_t colors[3] = {0xFF0000, 0x00FF00, 0x555555};

uint32_t rainbowOrder(byte position);
void clearLEDs();
void cylon(unsigned long color, byte wait);
void cascade(unsigned long color, byte direction, byte wait);
void rainbow(byte startPosition);

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MCP23017 mcp;
MCP23017PinBank pinBank = MCP23017PinBank(mcp);
RotaryEncoder* rotary;
uint8_t red = 255;
volatile boolean awakenByInterrupt = false;

void callback() {
  //rotary->service();
  awakenByInterrupt = true;
}

void setup()
{
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
  Serial.begin(9600);


  mcp.begin(0);
  mcp.setupInterrupts(true,false,LOW);

  mcp.pinMode(0, INPUT);
  mcp.pullUp(0, HIGH);
  mcp.setupInterruptPin(0, CHANGE);

  mcp.pinMode(1, INPUT);
  mcp.pullUp(1, HIGH);
  mcp.setupInterruptPin(1, CHANGE);
  /*
  mcp.pinMode(2, INPUT);
  mcp.pullUp(2, HIGH);
  mcp.setupInterruptPin(2, FALLING);
  */

  rotary = new RotaryEncoder(0, 1, &pinBank);

  /*
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  */
}


int last = 0;
void handleInterrupt() {
	  /*
	  uint8_t pin=mcp.getLastInterruptPin();
	  uint8_t val=mcp.getLastInterruptPinValue();
	  Serial.print(pin); Serial.print(": "); Serial.println(val);
	  */
      int a = pinBank.digitalRead(0);
	  int b = pinBank.digitalRead(1);
	  if (a != b) {
		  last = a;
	  } else {
		  if (a == 0) {
			  if (last == 1) {
				  Serial.println("RIGHT");
			  } else {
				  Serial.println("LEFT");
			  }
		  } else {
			  if (last == 0) {
				  Serial.println("RIGHT");
			  } else {
				  Serial.println("LEFT");
			  }
		  }
	  }
	  awakenByInterrupt = false;
}

void loop() {
	  // enable interrupts before going to sleep/wait
	  // And we setup a callback for the arduino INT handler.
	  //EIFR = (1<< INTF0);
	  attachInterrupt(0,callback,FALLING);

	  // Simulate a deep sleep
	  while(!awakenByInterrupt);
	  // Or sleep the arduino, this lib is great, if you have it.
	  //LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

	  // disable interrupts while handling them.
	  detachInterrupt(0);

	  if(awakenByInterrupt) handleInterrupt();

	/*
	rotary->service();
	delay(100);
	int16_t val = rotary->getValue();
    Serial.println(val);
	*/

	/*
	uint8_t val = (uint8_t)((exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0);
	leds.setBrightness(val);
    for (int i=0; i<LED_COUNT; i++) {
    	leds.setPixelColor(i, red, 100, 100);
    }
    leds.show();
    delay(10);
    */
	/*
	// Ride the Rainbow Road
  for (int i=0; i<LED_COUNT*10; i++)
  {
    rainbow(i);
    delay(100);  // Delay between rainbow slides
  }



  // Indigo cylon
  // Do a cylon (larson scanner) cycle 10 times
  for (int i=0; i<10; i++)
  {
    // cylon function: first param is color, second is time (in ms) between cycles
    cylon(INDIGO, 500);  // Indigo cylon eye!
  }

  // A light shower of spring green rain
  // This will run the cascade from top->bottom 20 times
  for (int i=0; i<20; i++)
  {
    // First parameter is the color, second is direction, third is ms between falls
    cascade(MEDIUMSPRINGGREEN, TOP_DOWN, 100);
  }
  */
}

// Implements a little larson "cylon" sanner.
// This'll run one full cycle, down one way and back the other
void cylon(unsigned long color, byte wait)
{
  // weight determines how much lighter the outer "eye" colors are
  const byte weight = 4;
  // It'll be easier to decrement each of these colors individually
  // so we'll split them out of the 24-bit color value
  byte red = (color & 0xFF0000) >> 16;
  byte green = (color & 0x00FF00) >> 8;
  byte blue = (color & 0x0000FF);

  // Start at closest LED, and move to the outside
  for (int i=0; i<=LED_COUNT-1; i++)
  {
    clearLEDs();
    leds.setPixelColor(i, red, green, blue);  // Set the bright middle eye
    // Now set two eyes to each side to get progressively dimmer
    for (int j=1; j<3; j++)
    {
      if (i-j >= 0)
        leds.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
      if (i-j <= LED_COUNT)
        leds.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
    }
    leds.show();  // Turn the LEDs on
    delay(wait);  // Delay for visibility
  }

  // Now we go back to where we came. Do the same thing.
  for (int i=LED_COUNT-2; i>=1; i--)
  {
    clearLEDs();
    leds.setPixelColor(i, red, green, blue);
    for (int j=1; j<3; j++)
    {
      if (i-j >= 0)
        leds.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
      if (i-j <= LED_COUNT)
        leds.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
      }

    leds.show();
    delay(wait);
  }
}

// Cascades a single direction. One time.
void cascade(unsigned long color, byte direction, byte wait)
{
  if (direction == TOP_DOWN)
  {
    for (int i=0; i<LED_COUNT; i++)
    {
      clearLEDs();  // Turn off all LEDs
      leds.setPixelColor(i, color);  // Set just this one
      leds.show();
      delay(wait);
    }
  }
  else
  {
    for (int i=LED_COUNT-1; i>=0; i--)
    {
      clearLEDs();
      leds.setPixelColor(i, color);
      leds.show();
      delay(wait);
    }
  }
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position.
// ROY G BIV!
void rainbow(byte startPosition)
{
  // Need to scale our rainbow. We want a variety of colors, even if there
  // are just 10 or so pixels.
  int rainbowScale = 192 / LED_COUNT;

  // Next we setup each pixel with the right color
  for (int i=0; i<LED_COUNT; i++)
  {
    // There are 192 total colors we can get out of the rainbowOrder function.
    // It'll return a color between red->orange->green->...->violet for 0-191.
    leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
  }
  // Finally, actually turn the LEDs on:
  leds.show();
}

// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position)
{
  // 6 total zones of color change:
  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
}
