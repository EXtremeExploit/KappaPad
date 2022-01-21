#include "pico/stdlib.h"
#include <stdint.h>

#define DIRECT_MODE_OUTPUT(pin) gpio_set_dir(pin, 1)
#define DIRECT_MODE_INPUT(pin)  gpio_set_dir(pin, 0)

#define totalTimeout 31000

struct CapacitiveSensor {
	public:
	CapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
	long SenseOneCycle();
	long total;

	uint8_t rPin;
	uint8_t sPin;
	// methods
};
