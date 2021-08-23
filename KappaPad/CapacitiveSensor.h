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
#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if !defined(__AVR__)
#include "pico/stdlib.h"
#else
// Direct I/O through registers and bitmask (from OneWire library)

#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask), (*((base)+2)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))
#endif
// library interface description
class CapacitiveSensor
{
  // user-accessible "public" interface
  public:
  // methods
	CapacitiveSensor(uint8_t sendPin, uint8_t receivePin);
	long capacitiveSensorRaw();
  // library-accessible "private" interface
  private:
  // variables
	int error;
	unsigned long  leastTotal;
	unsigned int   loopTimingFactor;
	unsigned long  CS_Timeout_Millis;
	unsigned long  CS_AutocaL_Millis;
	unsigned long  lastCal;
	unsigned long  total;
	IO_REG_TYPE sBit;   // send pin's ports and bitmask
	volatile IO_REG_TYPE *sReg;
	IO_REG_TYPE rBit;    // receive pin's ports and bitmask
	volatile IO_REG_TYPE *rReg;
    #if !defined(__AVR__)
    uint8_t rPin;
    uint8_t sPin;
    #endif
  // methods
	void SenseOneCycle(void);
};
