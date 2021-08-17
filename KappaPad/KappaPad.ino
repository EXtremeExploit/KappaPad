#include "config.h"
#include "digitalWriteFast.h"
#include "CapacitiveSensor.h"
#include "Keyboard.h"
#include "CapacitiveKey.h"

int main() {
	init();

	CapacitiveKey key0 = {
		key0SendPin,   //Capacitive Send Pin
		key0SensePin,  //Capacitive Sense Pin
		key0Threshold,   //Capacitive Threshold
		key0Debounce, // Key Debounce
		key0char  //Keyboard Key
	};

	CapacitiveKey key1 = {
		key1SendPin,   //Capacitive Send Pin
		key1SensePin,  //Capacitive Sense Pin
		key1Threshold,   //Capacitive Threshold
		key1Debounce, // Key Debounce
		key1char  //Keyboard Key
	};

	checkConfig(key0,key1);

#if defined(USBCON)
	USBDevice.attach();
#endif

#if defined(SERIAL_OUTPUT)
	Serial.begin(115200);
#endif

	bool kbEnable = false;

	while(1) {
		kbEnable = Keyboard.getLedStatus() & 0b10;

#if defined(SERIAL_OUTPUT)
		key0.keyUpdate(kbEnable);
		key1.keyUpdate(kbEnable);

		if(key0.sample >= 0 && key1.sample >= 0) {
			char* key0Sample = "";
			char* key1Sample = "";

			if(key0.sample < 10)
				key0Sample = '0' + String(key0.sample).c_str();
			if(key1.sample < 10)
				key1Sample = '0' + String(key1.sample).c_str();

			Serial.print(String(key0.sample));
			Serial.print('-');
			Serial.println(String(key1.sample));
		} else {
			if(key0.sample < 0)
				Serial.println("Key0 throw error: " + String(key0.sample));
			if(key1.sample < 0)
				Serial.println("Key1 throw error: " + String(key1.sample));
		}

		uint8_t ledStatus = Keyboard.getLedStatus();
		//Serial.print("ledStatus: ");
		//Serial.println(ledStatus, BIN);

		//Serial.print("kbEnable:  ");
		//Serial.println(kbEnable, BIN);
#else
		if(kbEnable) {
			key0.keyUpdate();
			key1.keyUpdate();
		} else {
			if(!key0.keyReleased) {
				Keyboard.release(key0.key);
				key0.keyReleased = true;
			}
			if(!key1.keyReleased) {
				Keyboard.release(key1.key);
				key1.keyReleased = true;
			}
		}
#endif

		if (kbEnable) {
			digitalWriteFast(onOffLED, HIGH);
		} else {
			digitalWriteFast(onOffLED, LOW);
		}
	}
}

void checkConfig(CapacitiveKey key0,CapacitiveKey key1){
	if (key0.rawKey < 'a' || key0.rawKey > 'z' ||
		key1.rawKey < 'a' || key1.rawKey > 'z' ||
		key0.sendPin == onOffLED || key0.sensePin == onOffLED ||
		key1.sendPin == onOffLED || key1.sensePin == onOffLED)
			while (1) digitalWrite(onOffLED, (millis() / 1000) % 2);
}