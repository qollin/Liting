/*
 * RotaryEncoder.h
 *
 *  Created on: Feb 13, 2015
 *      Author: collin
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include "PinBank.h"
#include <stdint.h>

class RotaryEncoder : public InterruptListener {
public:
	RotaryEncoder(uint8_t pinA, uint8_t pinB, PinBank* pinBank);
	virtual const void handleInterrupt(uint8_t pin, uint8_t value);
	int getValue() const;
	void setValue(int _value);
	void setCap(int minValue, int maxValue);
private:
	int cap(int i);
	uint8_t pinA, pinB;
	PinBank* pinBank;
	uint8_t lastValueA;
	int value;
	int minValue, maxValue;
	bool isCapped;
};

#endif /* ROTARYENCODER_H_ */
