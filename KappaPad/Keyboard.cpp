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
	0x05, 0x01,					// USAGE_PAGE (Generic Desktop)  // 47
	0x09, 0x06,					// USAGE (Keyboard)
	0xa1, 0x01,					// COLLECTION (Application)
	0x85, 0x02,					//   REPORT_ID (2)
	0x05, 0x07,					//   USAGE_PAGE (Keyboard)

  0x19, 0xe0,					//   USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7,					//   USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00,					//   LOGICAL_MINIMUM (0)
	0x25, 0x01,					//   LOGICAL_MAXIMUM (1)
	0x75, 0x01,					//   REPORT_SIZE (1)

  0x95, 0x08,					//   REPORT_COUNT (8)
	0x81, 0x02,					//   INPUT (Data,Var,Abs)
	0x95, 0x01,					//   REPORT_COUNT (1)
	0x75, 0x08,					//   REPORT_SIZE (8)
	0x81, 0x03,					//   INPUT (Cnst,Var,Abs)

	0x95, 0x05,                    //   REPORT_COUNT (5)
      0x75, 0x01,                    //   REPORT_SIZE (1)
      0x05, 0x08,                    //   USAGE_PAGE (LEDs)
      0x19, 0x01,                    //   USAGE_MINIMUM (1)
      0x29, 0x05,                    //   USAGE_MAXIMUM (5)
      0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) // LED report
      0x95, 0x01,                    //   REPORT_COUNT (1)
      0x75, 0x03,                    //   REPORT_SIZE (3)
      0x91, 0x01,                    //   OUTPUT (Constant) // padding

  0x95, 0x06,					//   REPORT_COUNT (6)
	0x75, 0x08,					//   REPORT_SIZE (8)
	0x15, 0x00,					//   LOGICAL_MINIMUM (0)
	0x25, 0x73,					//   LOGICAL_MAXIMUM (115)
	0x05, 0x07,					//   USAGE_PAGE (Keyboard)

  0x19, 0x00,					//   USAGE_MINIMUM (Reserved (no event indicated))
	0x29, 0x73,					//   USAGE_MAXIMUM (Keyboard Application)
	0x81, 0x00,					//   INPUT (Data,Ary,Abs)
	0xc0,						  // END_COLLECTION
};

Keyboard_::Keyboard_(void) {
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}


inline void sendReport(KeyReport* keys) __attribute__((always_inline));
void Keyboard_::sendReport(KeyReport* keys)
{
	HID().SendReport(2, keys, sizeof(KeyReport));
}

// press() adds the specified key to the persistent key report and sends it.
// Because of the way USB HID works,
// the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
void Keyboard_::press(uint8_t k) {
	if(_keyReport.keys[0] == 0x00)
		_keyReport.keys[0] = k - 93;
	else _keyReport.keys[1] = k - 93;

	sendReport(&_keyReport);
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
void Keyboard_::release(uint8_t k) {
	if(_keyReport.keys[0] == k - 93)
		_keyReport.keys[0] = 0x00;
	else _keyReport.keys[1] = 0x00;

	sendReport(&_keyReport);
}

inline uint8_t getLedStatus() __attribute__((always_inline));
uint8_t Keyboard_::getLedStatus()
{
	return HID().getKeyboardLedsStatus();
}

Keyboard_ Keyboard;

#endif

