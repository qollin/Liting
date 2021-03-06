/*
 * RotaryEncoder.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: collin
 */

#include <RotaryEncoder.h>
#include <Arduino.h>

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB, PinBank* pinBank) :
		pinA(pinA), pinB(pinB), pinBank(pinBank), lastValueA(0), value(0), minValue(
				0), maxValue(0), isCapped(false) {
}

void RotaryEncoder::setCap(int _minValue, int _maxValue) {
	minValue = _minValue;
	maxValue = _maxValue;
	isCapped = true;
}

void RotaryEncoder::setValue(int _value) {
	value = _value;
}

int RotaryEncoder::cap(int i) {
	return isCapped ? max(minValue, min(i, maxValue)) : i;
}

const void RotaryEncoder::handleInterrupt(uint8_t pin, uint8_t valueFromInterrupt) {
	uint8_t a = pinBank->digitalRead(pinA);
	uint8_t b = pinBank->digitalRead(pinB);

	if (a != b) {
		lastValueA = a;
	} else {
		if (a == 0) {
			if (lastValueA == 1) {
				value = cap(value + 1);
			} else {
				value = cap(value - 1);
			}
		} else {
			if (lastValueA == 0) {
				value = cap(value + 1);
			} else {
				value = cap(value - 1);
			}
		}
	}
}

int RotaryEncoder::getValue() const {
	return value;
}
