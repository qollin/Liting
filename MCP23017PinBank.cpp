#include "MCP23017PinBank.h"
#include <Arduino.h>

MCP23017PinBank::MCP23017PinBank(Adafruit_MCP23017 mcp) : mcp(mcp) {
	for (int i = 0; i < NUMBER_OF_PINS; i++) {
		listeners[i] = 0;
	}
}

const void MCP23017PinBank::setup() {
	mcp.begin(0);
	mcp.setupInterrupts(true,false,LOW);
}

const int MCP23017PinBank::digitalRead(uint8_t pin) {
	return mcp.digitalRead(pin);
}

const void MCP23017PinBank::digitalWrite(uint8_t pin, uint8_t value) {
	mcp.digitalWrite(pin, value);
}

const void MCP23017PinBank::addInterruptListener(uint8_t pin, InterruptListener* listener) {
	mcp.pinMode(pin, INPUT);
	mcp.pullUp(pin, HIGH);
	mcp.setupInterruptPin(pin, CHANGE);
	listeners[pin] = listener;
}

const void MCP23017PinBank::handleInterrupt() {
	uint8_t pin = mcp.getLastInterruptPin();
	uint8_t value = mcp.getLastInterruptPinValue(); //this clears the interrupt; otherwise the mcp stops sending interrupts
	if (pin < NUMBER_OF_PINS && listeners[pin] != 0) {
		listeners[pin]->handleInterrupt(pin, value);
	}
}
