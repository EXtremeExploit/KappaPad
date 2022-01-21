#include "tusb.h"

static uint8_t keycode[2] = {0};

void send_report() {
	tud_hid_n_report(0, 1, &keycode, sizeof(keycode));
}

void pressKey(uint8_t k) {
	if (keycode[0] == 0x00)
		keycode[0] = k;
	else
		keycode[1] = k;

	send_report();
}

void releaseKey(uint8_t k) {
	if (keycode[0] == k)
		keycode[0] = 0x00;
	else
		keycode[1] = 0x00;

	send_report();
}
