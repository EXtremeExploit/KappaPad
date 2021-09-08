#include "CapacitiveSensor.h"

#if defined(__AVR__)
#include "ArduinoKeyboard.h"
#else
#include "TUSBKeyboard.h"
#endif

struct CapacitiveKey {
	CapacitiveSensor* sensor;
	bool keyReleased = true;
	uint8_t key;
	char rawKey;

	unsigned int debounceDefault;
	unsigned int debounce;
	unsigned int threshold;
	long sample;
	uint8_t sendPin;
	uint8_t sensePin;
	CapacitiveKey(uint8_t _sendPin,
				  uint8_t _sensePin,
				  unsigned int capacitiveThreshold,
				  unsigned int _debounce,
				  char keyboardKey);

	~CapacitiveKey();

#if defined(SERIAL_DEBUG)
	void keyUpdate(bool kbEnabled);
#else
	void keyUpdate();
#endif
};
