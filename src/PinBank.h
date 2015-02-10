#ifndef __have__PinBank_h__
#define __have__PinBank_h__

#include <stdint.h>

class PinBank {
public:
	virtual const int digitalRead(uint8_t pin) = 0;
	virtual const void digitalWrite(uint8_t pin, uint8_t value) = 0;
	virtual const void pinMode(uint8_t pin, uint8_t mode) = 0;
};
#endif
