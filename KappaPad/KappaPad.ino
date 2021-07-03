#include "CapacitiveSensor.h"
#include "Keyboard.h"

#include "CapacitiveKey.h"

//#define SERIAL_OUTPUT


int main() {
	init();

#if defined(USBCON)
	USBDevice.attach();
#endif

#if defined(SERIAL_OUTPUT)
	Serial.begin(115200);
#endif

	CapacitiveKey key0 = {
		3,   //Capacitive Send Pin
		A2,  //Capacitive Sense Pin
		5,   //Capacitive Treshold
		'k'  //Keyboard Key
	};

	CapacitiveKey key1 = {
		2,   //Capacitive Send Pin
		A0,  //Capacitive Sense Pin
		6,   //Capacitive Treshold
		'l'  //Keyboard Key
	};

	bool kbEnable = false;


	if (key0.rawKey < 'a' || key0.rawKey > 'z' ||
	key1.rawKey < 'a' || key1.rawKey > 'z' ||
	key0.sendPin == LED_BUILTIN || key0.sensePin == LED_BUILTIN ||
	key1.sendPin == LED_BUILTIN || key1.sensePin == LED_BUILTIN)
		while (1) {
			digitalWrite(LED_BUILTIN, (millis() / 1000) % 2);
		}

	while(1) {
		kbEnable = Keyboard.getLedStatus() & 0b10;

#if defined(SERIAL_OUTPUT)
		key0.keyUpdate(kbEnable);
		key1.keyUpdate(kbEnable);
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
			#if defined(__AVR_ATmega32U4__)
				PORTC |= 0b10000000;
			#else
				digitalWrite(LED_BUILTIN, HIGH);
			#endif
		} else {
			#if defined(__AVR_ATmega32U4__)
				PORTC &= 0b01111111;
			#else
				digitalWrite(LED_BUILTIN, LOW);
			#endif
		}

	#if defined(SERIAL_OUTPUT)
		Serial.println(String(key0.sample) + "-" + String(key1.sample));

		uint8_t ledStatus = Keyboard.getLedStatus();
		Serial.print("ledStatus: ");
		Serial.println(ledStatus, BIN);

		Serial.print("kbEnable:  ");
		Serial.println(kbEnable, BIN);
	#endif
	}
}
