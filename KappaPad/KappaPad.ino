#include "CapacitiveSensor.h"
#include "Keyboard.h"

#include "CapacitiveKey.h"

//#define SERIAL_OUTPUT

void setup() {
#ifdef SERIAL_OUTPUT
	Serial.begin(115200);
#endif
	Keyboard.begin();
}

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

void loop() {
	bool kbEnable = Keyboard.getLedStatus() & 0b10;
	key0.keyUpdate(kbEnable);
	key1.keyUpdate(kbEnable);

	if (kbEnable) {
		//PORTB = PORTB | 0b00100000;
		digitalWrite(13, HIGH);
	}else{
		//PORTB = PORTB & 0b11011111;
		digitalWrite(13, LOW);
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
