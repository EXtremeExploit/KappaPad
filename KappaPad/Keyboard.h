/*
	Keyboard.h

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

#ifndef KEYBOARD_h
#define KEYBOARD_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Keyboard

//  Low level key report: up to 2 keys at once
typedef struct
{
	uint8_t modifiers;
	uint8_t reserved;
	uint8_t keys[2];
} KeyReport;

class Keyboard_
{
private:
	KeyReport _keyReport;
	void sendReport(KeyReport* keys);
public:
	Keyboard_(void);
	void press(uint8_t k);
	void release(uint8_t k);
	uint8_t getLedStatus();
};
extern Keyboard_ Keyboard;

#endif
#endif

