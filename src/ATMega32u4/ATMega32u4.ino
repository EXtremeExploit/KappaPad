#include "config.h"

#if onOffLED != -1
#if defined(__AVR__)
#include "digitalWriteFast.h"
#endif
#endif

#include "CapacitiveKey.h"

int main() {
	init();
	USBDevice.attach();

	if (key0char < 'a' || key0char > 'z' || key1char < 'a' || key1char > 'z' || key0SendPin == onOffLED || key0SensePin == onOffLED || key1SendPin == onOffLED || key1SensePin == onOffLED)
		while (1) digitalWriteFast(onOffLED, (millis() / 1000) % 2);

	CapacitiveKey key0 = {
		key0SendPin,   // Capacitive Send Pin
		key0SensePin,  // Capacitive Sense Pin
		key0Threshold, // Capacitive Threshold
		key0Debounce,  // Key Debounce
		key0char,      // Keyboard Key
		0              // Key ID
	};

	CapacitiveKey key1 = {
		key1SendPin,   // Capacitive Send Pin
		key1SensePin,  // Capacitive Sense Pin
		key1Threshold, // Capacitive Threshold
		key1Debounce,  // Key Debounce
		key1char,      // Keyboard Key
		1              // Key ID
	};

	bool kbEnable = false;

	char buf[128];
	unsigned long start;
	unsigned long end;
	unsigned long time_diff;

	while (1) {
		kbEnable = Keyboard.getLedStatus() & KEY_TOGGLE;

#if defined(DEBUG)
		start = micros();
		key0.keyUpdate(kbEnable);
		key1.keyUpdate(kbEnable);
		end = micros();

		time_diff = end - start;

		sprintf(buf, "[%20d] [Delta: %4lld] [K0S: %4ld] [K0D: %5d] [K0R: %d] [k1S: %4ld] [K1D: %5d] [K1R: %d]\n",
				micros(), time_diff, key0.sample, key0.debounce, key0.keyReleased, key1.sample, key1.debounce, key1.keyReleased);
		Serial.print(buf);
#else
		if (kbEnable) {
			key0.keyUpdate();
			key1.keyUpdate();
		} else {
			if (!key0.keyReleased)
				key0.release();
			if (!key1.keyReleased)
				key1.release();
		}
#endif

#if onOffLED != -1
#if defined(__AVR__)
		if (kbEnable) {
			digitalWriteFast(onOffLED, HIGH);
		} else {
			digitalWriteFast(onOffLED, LOW);
		}
#endif
#endif
	}
}
