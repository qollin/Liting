/*
 * PushButton.cpp
 *
 *  Created on: Feb 21, 2015
 *      Author: collin
 */

#include "PushButton.h"

PushButton::PushButton():
	pressed(false) {
}

const void PushButton::handleInterrupt(uint8_t pin, uint8_t value) {
	pressed = value;
}

const bool PushButton::isPressed() const {
	return pressed;
}

