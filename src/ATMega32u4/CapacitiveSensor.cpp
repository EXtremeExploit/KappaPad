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

#include "CapacitiveSensor.h"

#define totalTimeout 620000

// Constructor
// Function that handles the creation and setup of instances

CapacitiveSensor::CapacitiveSensor(uint8_t sendPin, uint8_t receivePin) {
	//CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;

	sBase = PIN_TO_BASEREG(sendPin);  // get pointer to output register
	sMask = PIN_TO_BITMASK(sendPin);  // get send pin's ports and bitmask
	DIRECT_MODE_OUTPUT(sBase, sMask); // sendpin to OUTPUT

	rBase = PIN_TO_BASEREG(receivePin);
	rMask = PIN_TO_BITMASK(receivePin); // get receive pin's ports and bitmask
}

long CapacitiveSensor::SenseOneCycle() {
	total = 0;

	// set rPin HIGH briefly to charge up fully
	DIRECT_MODE_OUTPUT(rBase, rMask); // rPin OUTPUT - pin is now HIGH AND OUTPUT
	DIRECT_WRITE_HIGH(rBase, rMask);

	DIRECT_MODE_INPUT(rBase, rMask); // rPin INPUT (pullup is off)
	DIRECT_WRITE_LOW(sBase, sMask);  // sPin LOW

	while (DIRECT_READ(rBase, rMask) && (total < totalTimeout)) // while rPin is HIGH
		total++;

	return total < totalTimeout ? total : -1;
}
