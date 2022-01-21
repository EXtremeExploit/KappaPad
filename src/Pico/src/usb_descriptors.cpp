/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdint.h>
#include "tusb.h"

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
	{
		.bLength         = sizeof(tusb_desc_device_t),
		.bDescriptorType = TUSB_DESC_DEVICE,
		.bcdUSB          = 0x0200,
		.bDeviceClass    = 0xEF,
		.bDeviceSubClass = 0x02,
		.bDeviceProtocol = 0x01,
		.bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

		.idVendor  = 0x7272,
		.idProduct = 0x0002,
		.bcdDevice = 0x0100,

		.iManufacturer = 0x01,
		.iProduct      = 0x02,
		.iSerialNumber = 0x03,

		.bNumConfigurations = 0x01};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const* tud_descriptor_device_cb(void) {
	return (uint8_t const*)&desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_hid_report[] =
	{
		//  Keyboard
		0x05, 0x01, // Usage page  1 (Generic Desktop)
		0x09, 0x06, // Usage 06 (Keyboard)
		0xa1, 0x01, // COLLECTION (Application)

		// Setup
		0x85, 0x01, // REPORTID
					// Actual keys
		0x05, 0x07, // Usage page: Keyboard
		0x19, 0x00, // Usage min: 0
		0x29, 0x73, // Usage max 115 (F24)
		0x15, 0x00, // Logical min: 0
		0x25, 0x73, // Logical max: 115
		0x95, 0x02, // Report Count: 2 reports
		0x75, 0x08, // Report size: 8 bits (1 byte)
		0x81, 0x00, // INPUT (Data,Ary,Abs)
					// LEDs
		0x05, 0x08, // Usage page (LEDs)
		0x19, 0x01, // Usage min: 1
		0x29, 0x05, // Usage max: 5
		0x95, 0x05, // Report Count: 5 reports
		0x75, 0x01, // Report size: 1 bit
		0x91, 0x02, // OUTPUT (Data,Var,Abs) // LED report
					// LEDs padding
		0x95, 0x01, // Report Count: 1 report
		0x75, 0x03, // Report size: 3 bits
		0x91, 0x01, // OUTPUT (Constant)

		0xc0, // END_COLLECTION
};
// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb(uint8_t itf) {
	(void)itf;
	return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum {
	ITF_NUM_HID,
	ITF_NUM_CDC,
	ITF_NUM_CDC_DATA,
	ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_CDC_DESC_LEN)

#define EPNUM_HID 0x84

#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x82

uint8_t const desc_configuration[] =
	{
		// Config number, interface count, string index, total length, attribute, power in mA
		TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

		// Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
		TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 1),

		// Interface number, string index, EP notification address and size, EP data address (out, in) and size.
		TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64)};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
	(void)index; // for multiple configurations
	return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// Array of pointer to string descriptors
// Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
// https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
char const* string_desc_arr[] =
	{
		(const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
		"EXtremeExploit",           // 1: Manufacturer
		"KappaPad",                 // 2: Product
		"RP2040 Pico",              // 3: Serials, should use chip ID
		"KappaPad Serial",          // 4: CDC Interface
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void)langid;

	uint8_t chr_count = 1;

	if (index == 0) {
		memcpy(&_desc_str[1], string_desc_arr[0], 2);
	} else {

		if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) return NULL;

		const char* str = string_desc_arr[index];

		// Cap at max char
		chr_count = strlen(str);
		if (chr_count > 31) chr_count = 31;

		// Convert ASCII string into UTF-16
		for (uint8_t i = 0; i < chr_count; i++) {
			_desc_str[1 + i] = str[i];
		}
	}

	// first byte is length (including header), second byte is string type
	_desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

	return _desc_str;
}
