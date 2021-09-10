/*
  CapacitiveSense.h v.04 - Capacitive Sensing Library for 'duino / Wiring
  https://github.com/PaulStoffregen/CapacitiveSensor
  http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
  http://playground.arduino.cc/Main/CapacitiveSensor
  Copyright (c) 2008 Paul Bagder  All rights reserved.
  Version 05 by Paul Stoffregen - Support non-AVR board: Teensy 3.x, Arduino Due
  Version 04 by Paul Stoffregen - Arduino 1.0 compatibility, issue 146 fix
  vim: set ts=4:
*/

// ensure this library description is only included once

// Direct I/O through registers and bitmask (from OneWire library)

#if defined(ARDUINO)
#include "Arduino.h"
#define PIN_TO_BASEREG(pin)            (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)            (digitalPinToBitMask(pin))
#define DIRECT_READ(base, mask)        (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)  ((*((base) + 1)) &= ~(mask), (*((base) + 2)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask) ((*((base) + 1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)   ((*((base) + 2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)  ((*((base) + 2)) |= (mask))
#endif

class CapacitiveSensor {
	public:
	CapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
	long SenseOneCycle();
	long total;

	uint8_t sMask; // send pin's ports and bitmask
	uint8_t* sBase;

	uint8_t rMask; // receive pin's ports and bitmask
	uint8_t* rBase;

#if !defined(__AVR__)
	uint8_t rPin;
	uint8_t sPin;
#endif
	// methods
};
