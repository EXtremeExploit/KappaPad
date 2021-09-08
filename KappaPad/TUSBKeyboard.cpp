#if !defined(__AVR__)
#include "TUSBKeyboard.h"

#define RID_KEYBOARD 1

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] =
	{
		TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD))};
Adafruit_USBD_HID usb_hid;

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	//  Keyboard
	0x05, 0x01, // Usage page  1 (Generic Desktop)
	0x09, 0x07, // Usage 06 (Keypad)
	0xa1, 0x01, // COLLECTION (Application)

	// Setup
	0x85, 0x02, // REPORT_ID (2)
	0x05, 0x07, // Usage page (Keyboard)

	// LEDs
	0x95, 0x05, // Report Count: 5 reports
	0x75, 0x01, // Report size: 1 bit
	0x05, 0x08, // Usage page (LEDs)
	0x19, 0x01, // Usage min: 1
	0x29, 0x05, // Usage max: 5
	0x91, 0x02, // OUTPUT (Data,Var,Abs) // LED report

	// LEDs padding
	0x95, 0x01, // Report Count: 1 report
	0x75, 0x03, // Report size: 3 bits
	0x91, 0x01, // OUTPUT (Constant)

	// Actual keys
	0x95, 0x02, // Report Count: 2 reports
	0x75, 0x08, // Report size: 8 bits (1 byte)
	0x25, 0x73, // Logical max: 115
	0x05, 0x07, // Usage page: Keyboard
	0x19, 0x00, // Usage min: 0
	0x29, 0x73, // Usage max 115 (F24)
	0x81, 0x00, // INPUT (Data,Ary,Abs)

	0xc0, // END_COLLECTION
};

TinyKeyboard_::TinyKeyboard_(void) {
}

void TinyKeyboard_::begin(void) {
	usb_hid.setPollInterval(2);
	usb_hid.setReportDescriptor(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	usb_hid.begin();
	while (!USBDevice.mounted()) delay(1);
}

void TinyKeyboard_::end(void) {
}

void TinyKeyboard_::sendReport(Keys) {
	while (!usb_hid.ready()) delay(1);
	usb_hid.keyboardReport(RID_KEYBOARD, keys->keys);
	delay(2);
}

// press() adds the specified key to the persistent key report and sends it.
// Because of the way USB HID works,
// the host acts like the key remains pressed until we
// call release(), or otherwise clear the report and resend.
void TinyKeyboard_::press(uint8_t k) {
	if (keys[0] == 0x00)
		keys[0] = k;
	else
		keys[1] = k;

	sendReport(keys);
}

// release() takes the specified key out of the persistent key report and
// sends the report. This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
void TinyKeyboard_::release(uint8_t k) {
	if (keys[0] == k)
		keys[0] = 0x00;
	else
		keys[1] = 0x00;

	sendReport(keys);
}

TinyKeyboard_ Keyboard; //create an instance of the Keyboard object
#endif
