#include "config.h"

#include "bsp/board.h"

#include "tusb.h"
#include "tusb_option.h"

#include "CapacitiveKey.h"

enum BlinkInterval {
	NOT_MOUNTED = 250,
	ERROR       = 500,
	MOUNTED     = 1000,
	SUSPENDED   = 2500,

	ALWAYS_OFF = -1,
	ALWAYS_ON  = -2,
};

static uint32_t blink_interval_ms = BlinkInterval::NOT_MOUNTED;

void led_blinking_task();
void serial_print(const char* str);

bool serialReady = false;
bool kbEnable    = false;

int main() {
	board_init();
	tusb_init();

	if (key0char < 'a' || key0char > 'z' || key1char < 'a' || key1char > 'z') {
		while (1) {
			blink_interval_ms = BlinkInterval::ERROR;
			led_blinking_task();
		}
	}

	CapacitiveKey key0 = {
		key0SendPin,   // Capacitive Send Pin
		key0SensePin,  // Capacitive Sense Pin
		key0Threshold, // Capacitive Threshold
		key0Debounce,  // Key Debounce
		key0char,      // Keyboard Key
		0              // Key ID
	};

	CapacitiveKey key1 = {
		key1SendPin,   // Capacitive Send Pin
		key1SensePin,  // Capacitive Sense Pin
		key1Threshold, // Capacitive Threshold
		key1Debounce,  // Key Debounce
		key1char,      // Keyboard Key
		1              // Key ID
	};

	char buf[128];
	absolute_time_t start;
	absolute_time_t end;
	int64_t time_diff;

	while (1) {
		tud_task();
		if (!tud_hid_ready()) continue;

#if defined(DEBUG)
		start = get_absolute_time();
		key0.keyUpdate(kbEnable);
		key1.keyUpdate(kbEnable);
		end = get_absolute_time();

		time_diff = absolute_time_diff_us(start, end);

		sprintf(buf, "[%20d] [Delta: %4lld] [K0S: %4ld] [K0D: %5d] [K0R: %d] [k1S: %4ld] [K1D: %5d] [K1R: %d]\n",
				get_absolute_time(), time_diff, key0.sample, key0.debounce, key0.keyReleased, key1.sample, key1.debounce, key1.keyReleased);
		serial_print(buf);
#else
		if (kbEnable) {
			key0.keyUpdate();
			key1.keyUpdate();
		} else {
			if (!key0.keyReleased)
				key0.release();
			if (!key1.keyReleased)
				key1.release();
		}
#endif // defined(DEBUG)
		led_blinking_task();
	}
}

void tud_mount_cb(void) {
	blink_interval_ms = BlinkInterval::MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
	blink_interval_ms = BlinkInterval::NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
	(void)remote_wakeup_en;
	blink_interval_ms = BlinkInterval::SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
	blink_interval_ms = BlinkInterval::MOUNTED;
}

void led_blinking_task(void) {
	static uint32_t start_ms = 0;
	static bool led_state    = false;

	// blink is disabled
	if (!blink_interval_ms) return;

	if (blink_interval_ms == BlinkInterval::ALWAYS_ON && !led_state) {
		board_led_write(true);
		led_state = true;
		return;
	}

	if (blink_interval_ms == BlinkInterval::ALWAYS_OFF && led_state) {
		board_led_write(false);
		led_state = false;
		return;
	}

	// Blink every interval ms
	if (board_millis() - start_ms < blink_interval_ms) return; // not enough time
	start_ms += blink_interval_ms;

	board_led_write(led_state);
	led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
	// TODO not Implemented
	(void)itf;
	(void)report_id;
	(void)report_type;
	(void)buffer;
	(void)reqlen;

	return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
	// This example doesn't use multiple report and report ID
	(void)itf;
	(void)report_id;
	(void)report_type;

	if (bufsize == 2) {
		if (buffer[1] & KEY_TOGGLE) {
			blink_interval_ms = BlinkInterval::ALWAYS_ON;
			kbEnable          = true;
		} else {
			blink_interval_ms = BlinkInterval::ALWAYS_OFF;
			kbEnable          = false;
		}
	}

	// uint8_t status = 0;
	// memcpy(&status, buffer, 1);

	// echo back anything we received from host
	// tud_hid_report(0, buffer, bufsize);
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
	(void)itf;
	(void)rts;

	serialReady = false;
	if (dtr) {
		serialReady = true;
		tud_cdc_write_str("Connected\n");
	}
}

void serial_print(const char* str) {
	if (serialReady) {
		tud_cdc_write_str(str);
	}
}
