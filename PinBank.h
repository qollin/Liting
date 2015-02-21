#ifndef __have__PinBank_h__
#define __have__PinBank_h__

#include <stdint.h>
#include <Arduino.h>

class PinBank {
public:
	virtual ~PinBank() {}
	virtual const int digitalRead(uint8_t pin) = 0;
	virtual const void digitalWrite(uint8_t pin, uint8_t value) = 0;
};

class InterruptListener {
public:
	virtual ~InterruptListener() {}
	virtual const void handleInterrupt(uint8_t pin, uint8_t value) = 0;
};

class PushButton : public InterruptListener {
public:
	virtual ~PushButton() {}
	virtual const void handleInterrupt(uint8_t pin, uint8_t value) {
		pressed = value;
	}
	const bool isPressed() const {
		return pressed;
	}
private:
	bool pressed;
};
#endif
