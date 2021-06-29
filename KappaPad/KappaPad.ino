#include "CapacitiveSensor.h"
#include "Keyboard.h"

#include "CapacitiveKey.h"

//#define SERIAL_OUTPUT

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

void setup() {
#ifdef SERIAL_OUTPUT
	Serial.begin(115200);
#endif

	if (key0.key < 'a' || key0.key > 'z' ||
	key1.key < 'a' || key1.key > 'z' ||
	key0.sendPin == LED_BUILTIN || key0.sensePin == LED_BUILTIN ||
	key1.sendPin == LED_BUILTIN || key1.sensePin == LED_BUILTIN)
	{
		while (1) {
			digitalWrite(LED_BUILTIN, (millis() / 1000) % 2);
		}
	}
}

void loop() {
	kbEnable = Keyboard.getLedStatus() & 0b10;
	key0.keyUpdate(kbEnable);
	key1.keyUpdate(kbEnable);

	if (kbEnable) {
		#ifdef __AVR_ATmega32U4__
			PORTC |= 0b10000000;
		#else
			digitalWrite(LED_BUILTIN, HIGH);
		#endif
	} else {
		#ifdef __AVR_ATmega32U4__
			PORTC &= 0b01111111;
		#else
			digitalWrite(LED_BUILTIN, LOW);
		#endif
	}

#ifdef SERIAL_OUTPUT
	Serial.println(String(key0.sample) + "-" + String(key1.sample));

	uint8_t ledStatus = Keyboard.getLedStatus();
	Serial.print("ledStatus: ");
	Serial.println(ledStatus, BIN);

	Serial.print("kbEnable:  ");
	Serial.println(kbEnable, BIN);
#endif
}
