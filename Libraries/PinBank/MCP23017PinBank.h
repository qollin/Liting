#include "PinBank.h"
#include <Adafruit_MCP23017.h>

class MCP23017PinBank : public PinBank {
public:
	MCP23017PinBank(Adafruit_MCP23017 mcp);
	virtual ~MCP23017PinBank() {}
	virtual const void setup();
	virtual const int digitalRead(uint8_t pin);
	virtual const void digitalWrite(uint8_t pin, uint8_t value);
	virtual const void addInterruptListener(uint8_t pin, InterruptListener* listener);
	virtual const void handleInterrupt();
private:
	static const int NUMBER_OF_PINS = 16;
	Adafruit_MCP23017 mcp;
	InterruptListener* listeners[16];
};
