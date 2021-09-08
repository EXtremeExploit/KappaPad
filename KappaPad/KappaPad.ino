#include "config.h"

#if onOffLED != -1
#include "digitalWriteFast.h"
#endif

#include "CapacitiveKey.h"

int main() {
#if defined(ARDUINO)
	init();
	USBDevice.attach();
#else
	Keyboard.begin();
#endif

	if (key0char < 'a' || key0char > 'z' || key1char < 'a' || key1char > 'z' || key0SendPin == onOffLED || key0SensePin == onOffLED || key1SendPin == onOffLED || key1SensePin == onOffLED)
		while (1) digitalWrite(onOffLED, (millis() / 1000) % 2);

	CapacitiveKey key0 = {
		key0SendPin,   //Capacitive Send Pin
		key0SensePin,  //Capacitive Sense Pin
		key0Threshold, //Capacitive Threshold
		key0Debounce,  // Key Debounce
		key0char       //Keyboard Key
	};

	CapacitiveKey key1 = {
		key1SendPin,   //Capacitive Send Pin
		key1SensePin,  //Capacitive Sense Pin
		key1Threshold, //Capacitive Threshold
		key1Debounce,  // Key Debounce
		key1char       //Keyboard Key
	};

	bool kbEnable = false;

	while (1) {
		kbEnable = Keyboard.getLedStatus() & 0b10;

#if defined(SERIAL_DEBUG)
		auto start = micros();
		key0.keyUpdate(kbEnable);
		key1.keyUpdate(kbEnable);
		auto end = micros();

		Serial.println(String(end - start) + "us");

		if (key0.sample >= 0 && key1.sample >= 0) {
			if (key0.sample < 10)
				Serial.print(0);
			Serial.print(key0.sample);

			Serial.print('-');

			if (key1.sample < 10)
				Serial.print(0);
			Serial.println(key1.sample);
		} else {
			if (key0.sample < 0)
				Serial.println("Key0 throw error: " + String(key1.sample));
			if (key1.sample < 0)
				Serial.println("Key1 throw error: " + String(key1.sample));
		}

		// uint8_t ledStatus = Keyboard.getLedStatus();
		// Serial.print("ledStatus: ");
		// Serial.println(ledStatus, BIN);

		// Serial.print("kbEnable:  ");
		// Serial.println(kbEnable, BIN);
#else
		if (kbEnable) {
			key0.keyUpdate();
			key1.keyUpdate();
		} else {
			if (!key0.keyReleased) {
				Keyboard.release(key0.key);
				key0.keyReleased = true;
			}
			if (!key1.keyReleased) {
				Keyboard.release(key1.key);
				key1.keyReleased = true;
			}
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
