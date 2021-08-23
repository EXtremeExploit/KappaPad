/*
 CapacitiveSense.h v.04 - Capacitive Sensing Library for 'duino / Wiring
 https://github.com/PaulStoffregen/CapacitiveSensor
 http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
 http://playground.arduino.cc/Main/CapacitiveSensor
 Copyright (c) 2009 Paul Bagder  All right reserved.
 Version 05 by Paul Stoffregen - Support non-AVR board: Teensy 3.x, Arduino Due
 Version 04 by Paul Stoffregen - Arduino 1.0 compatibility, issue 146 fix
 vim: set ts=4:
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

#include "CapacitiveSensor.h"

#define CS_Timeout_Millis ((2000 * (float)310 * (float)F_CPU) / 16000000)

// Constructor
// Function that handles the creation and setup of instances

CapacitiveSensor::CapacitiveSensor(uint8_t sendPin, uint8_t receivePin)
{
	// initialize this instance's variables
	//Serial.begin(9600);		// for debugging
	//loopTimingFactor = 310;		// determined empirically -  a hack

	//CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;

	// Serial.print("timwOut =  ");

	// get pin mapping and port for send Pin - from PinMode function in core

	pinMode(sendPin, OUTPUT);						// sendpin to OUTPUT
	pinMode(receivePin, INPUT);						// receivePin to INPUT
	digitalWrite(sendPin, LOW);

#if defined(__AVR__)
	sBit = PIN_TO_BITMASK(sendPin);					// get send pin's ports and bitmask
	sReg = PIN_TO_BASEREG(sendPin);					// get pointer to output register

	rBit = PIN_TO_BITMASK(receivePin);				// get receive pin's ports and bitmask
	rReg = PIN_TO_BASEREG(receivePin);
#else
	sPin = sendPin;
	rPin = receivePin;
#endif
}

// Public Methods
// Functions available in Wiring sketches, this library, and other libraries

long CapacitiveSensor::capacitiveSensorRaw() //Return -1 if pins are wrong, -2 if failed to sense
{
	total = 0;
	SenseOneCycle();   // variable over timeout

	return total;
}

// Private Methods
// Functions only available to other functions in this library

void CapacitiveSensor::SenseOneCycle(void)
{
	#if defined(__AVR__)
    noInterrupts(); // Disable interrupts

	DIRECT_WRITE_LOW(sReg, sBit);	// sendPin low
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin Input (pullups are off)

	DIRECT_MODE_OUTPUT(rReg, rBit); // receivePin OUTPUT
	DIRECT_WRITE_LOW(rReg, rBit);	// receivePin LOW

	delayMicroseconds(10);			// 10 microseconds delay

	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin Input (pullups are off)
	DIRECT_WRITE_HIGH(sReg, sBit);	// sendPin High

    interrupts(); // Enable interrupts

	while (!DIRECT_READ(rReg, rBit)  && (total < CS_Timeout_Millis)) {  // while receive pin is LOW
		total++;
	}
	//Serial.print("SenseOneCycle(1): ");
	//Serial.println(total);

	if (total > CS_Timeout_Millis) {
		total = -1;         //  total variable over timeout
		return;
	}

	// set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
    noInterrupts();
	DIRECT_WRITE_HIGH(rReg, rBit);
	DIRECT_MODE_OUTPUT(rReg, rBit);  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
	DIRECT_WRITE_HIGH(rReg, rBit);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to INPUT (pullup is off)
	DIRECT_WRITE_LOW(sReg, sBit);	// sendPin LOW
    interrupts();

#ifdef FIVE_VOLT_TOLERANCE_WORKAROUND
	DIRECT_MODE_OUTPUT(rReg, rBit);
	DIRECT_WRITE_LOW(rReg, rBit);
	delayMicroseconds(10);
	DIRECT_MODE_INPUT(rReg, rBit);	// receivePin to INPUT (pullup is off)
#else
	while (DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis)) {  // while receive pin is HIGH
		total++;
	}
#endif
	//Serial.print("SenseOneCycle(2): ");
	//Serial.println(total);
	if (total >= CS_Timeout_Millis) {
		total = -2;     // total variable over timeout
	}

	#else // Pi pico

	gpio_put(sPin, 0);
	gpio_set_dir(rPin, 0);

	gpio_set_dir(rPin, 1);
	gpio_put(rPin, 0);

	delay_us(10);

	gpio_set_dir(rPin, 0);
	gpio_put(sPin, 1);

	while(gpio_get(rPin) && total < CS_Timeout_Millis)
	{
		total ++
	}

	if (total > CS_Timeout_Millis) {
		total = -1;         //  total variable over timeout
		return;
	}

	gpio_put(rPin, 1);
	gpio_set_dir(rPin, 1);
	gpio_put(rPin, 1);
	gpio_set_dir(rPin, 0);
	gpio_put(sPin, 0);

	while (gpio_get(rPin) && (total < CS_Timeout_Millis)) {  // while receive pin is HIGH
		total++;
	}
	if (total > CS_Timeout_Millis) {
		total = -1;         //  total variable over timeout
		return;
	}

	#endif

	return;
}
