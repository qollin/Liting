#include <PinBank.h>

class ArduinoPinBank : public PinBank {
public:
	virtual const int digitalRead(uint8_t pin);
	virtual const void digitalWrite(uint8_t pin, uint8_t value);
	virtual const void pinMode(uint8_t pin, uint8_t mode);
};
