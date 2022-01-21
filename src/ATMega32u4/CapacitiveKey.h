#include "CapacitiveSensor.h"

#include "Keyboard.h"

struct CapacitiveKey {
	uint8_t sendPin;
	uint8_t sensePin;
	unsigned int threshold;
	char stringKey;
	uint8_t id;

	uint8_t key;
	CapacitiveSensor* sensor;
	int debounceDefault;
	bool keyReleased = true;
	long sample;

	int debounce;

	CapacitiveKey(uint8_t _sendPin,
				  uint8_t _sensePin,
				  unsigned int capacitiveThreshold,
				  int _debounce,
				  char keyboardKey,
				  uint8_t _id);

	void keyUpdate(bool kbEnabled);
	void keyUpdate();

	void press();
	void release();
};
