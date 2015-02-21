/*
 * PushButton.h
 *
 *  Created on: Feb 21, 2015
 *      Author: collin
 */

#ifndef LIBRARIES_PINBANK_PUSHBUTTON_H_
#define LIBRARIES_PINBANK_PUSHBUTTON_H_

#include "PinBank.h"

class PushButton : public InterruptListener {
public:
	PushButton();
	virtual ~PushButton() {}
	virtual const void handleInterrupt(uint8_t pin, uint8_t value);
	virtual const bool isPressed() const;
private:
	bool pressed;
};

#endif /* LIBRARIES_PINBANK_PUSHBUTTON_H_ */
