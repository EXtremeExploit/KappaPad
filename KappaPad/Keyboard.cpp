/*
  Keyboard.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Keyboard.h"

#if defined(_USING_HID)

//================================================================================
//================================================================================
//	Keyboard

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	//  Keyboard
	0x05, 0x01,			// Usage page  1 (Generic Desktop)
	0x09, 0x07,			// Usage 06 (Keypad)
	0xa1, 0x01,			// COLLECTION (Application)

		// Setup
		0x85, 0x02,		// REPORT_ID (2)
		0x05, 0x07,		// Usage page (Keyboard)

		// Modifiers
		// 0x19, 0xe0,		// Usage min: E0
		// 0x29, 0xe7,		// Usage max: E7
		// 0x15, 0x00,		// Logical min: 0
		// 0x25, 0x01,		// Logical max: 1
		// 0x75, 0x01,		// Report size: 1 bit
		// 0x95, 0x08,		// Report Count (8)
		// 0x81, 0x02,		// INPUT (Data,Var,Abs)

		// Reserved byte
		// 0x95, 0x01,		// Report Count (1)
		// 0x75, 0x08,		// Report size (8)
		// 0x81, 0x03,		// INPUT (Cnst,Var,Abs)

		// LEDs
		0x95, 0x05,		// Report Count: 5 reports
		0x75, 0x01,		// Report size: 1 bit
		0x05, 0x08,		// Usage page (LEDs)
		0x19, 0x01,		// Usage min: 1
		0x29, 0x05,		// Usage max: 5
		0x91, 0x02,		// OUTPUT (Data,Var,Abs) // LED report

		// LEDs padding
		0x95, 0x01,		// Report Count: 1 report
		0x75, 0x03,		// Report size: 3 bits
		0x91, 0x01,		// OUTPUT (Constant)

		// Actual keys
		0x95, 0x02,		// Report Count: 2 reports
		0x75, 0x08,		// Report size: 8 bits (1 byte)
		0x15, 0x00,		// Logical min: 0
		0x25, 0x73,		// Logical max: 115
		0x05, 0x07,		// Usage page: Keyboard
		0x19, 0x00,		// Usage min: 0
		0x29, 0x73,		// Usage max 115 (F24)
		0x81, 0x00,		// INPUT (Data,Ary,Abs)

	0xc0,				// END_COLLECTION
};

Keyboard_::Keyboard_(void) {
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

inline void sendReport(Keys) __attribute__((always_inline));
void Keyboard_::sendReport(Keys)
{
	HID().SendReport(2, keys, sizeof(keys));
}

// press() adds the specified key to the persistent key report and sends it.
// Because of the way USB HID works,
// the host acts like the key remains pressed until we
// call release(), or otherwise clear the report and resend.
void Keyboard_::press(uint8_t k) {
	if (keys[0] == 0x00)
		keys[0] = k;
	else keys[1] = k;

	sendReport(keys);
}

// release() takes the specified key out of the persistent key report and
// sends the report. This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
void Keyboard_::release(uint8_t k) {
	if (keys[0] == k)
		keys[0] = 0x00;
	else keys[1] = 0x00;

	sendReport(keys);
}

inline uint8_t getLedStatus() __attribute__((always_inline));
uint8_t Keyboard_::getLedStatus()
{
	return HID()._keyboardLedsStatus;
}

Keyboard_ Keyboard;

#endif

