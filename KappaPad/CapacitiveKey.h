#include "CapacitiveSensor.h"

#include "ArduinoKeyboard.h"

struct CapacitiveKey {
	uint8_t sendPin;
	uint8_t sensePin;
	unsigned int threshold;
	char rawKey;
	uint8_t key;
	CapacitiveSensor* sensor;
	unsigned int debounceDefault;
	bool keyReleased = true;
	long sample;

	unsigned int debounce;
	CapacitiveKey(uint8_t _sendPin,
				  uint8_t _sensePin,
				  unsigned int capacitiveThreshold,
				  unsigned int _debounce,
				  char keyboardKey);

	void keyUpdate(bool kbEnabled);
	void keyUpdate();
};
