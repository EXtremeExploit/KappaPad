#include "CapacitiveSensor.h"

CapacitiveSensor::CapacitiveSensor(uint8_t sendPin, uint8_t receivePin) {
	gpio_init(sendPin);
	gpio_init(receivePin);

	gpio_set_dir(sendPin, 1);
	gpio_set_dir(receivePin, 0);

	sPin = sendPin;
	rPin = receivePin;
}

long CapacitiveSensor::SenseOneCycle() {
	total = 0;

	// set rPin HIGH briefly to charge up fully
	gpio_set_dir(rPin, 1);
	gpio_put(rPin, 1); // rPin OUTPUT - rpin is now HIGH AND OUTPUT

	gpio_pull_down(rPin);

	gpio_set_dir(rPin, 0); // rPin INPUT (pullup is off)
	gpio_put(sPin, 0);     // sPin LOW

	// Count the number of CPU cycles the Send Pin is HIGH for
	while (gpio_get(rPin) && (total < totalTimeout))
		total++;

	// Timeout
	if (total >= totalTimeout)
		return -2;

	return total;
}
