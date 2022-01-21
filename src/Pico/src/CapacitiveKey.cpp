#include "CapacitiveKey.h"

CapacitiveKey::CapacitiveKey(uint8_t sendPin,
							 uint8_t sensePin,
							 unsigned int capacitiveThreshold,
							 int _debounce,
							 char keyboardKey,
							 uint8_t _id) {
	threshold       = capacitiveThreshold;
	debounceDefault = _debounce;
	stringKey       = keyboardKey;
	id              = _id;

	sensor = new CapacitiveSensor(sendPin, sensePin);

	key = keyboardKey - 93;
}

void CapacitiveKey::keyUpdate(bool kbEnabled) {
	sample = sensor->SenseOneCycle();
	if (sample < 0)
		return;

#if defined(ENABLE_KEYS)
	if (kbEnabled) {
		if (sample > threshold) {
			if (keyReleased) {
				this->press();
			}
			debounce = debounceDefault;
		} else {
			if (!keyReleased) {
				if (debounce == 0) {
					this->release();
				} else {
					debounce--;
				}
			}
		}
	} else {
		if (!keyReleased)
			this->release();
	}
#endif // defined(ENABLE_KEYS)
}

void CapacitiveKey::keyUpdate() {
	sample = sensor->SenseOneCycle();
	if (sample > threshold) {
		if (keyReleased) {
			this->press();
		}
		debounce = debounceDefault;
	} else {
		if (!keyReleased) {
			if (debounce == 0) {
				this->release();
			} else {
				debounce--;
			}
		}
	}
}

void CapacitiveKey::press() {
	pressKey(key);
	keyReleased = false;
#if defined(DEBUG)
	char eBuff[64];
	sprintf(eBuff, "[%20d] ", get_absolute_time());
	serial_print(eBuff);
	sprintf(eBuff, "Key %d Pressed %c\n", id, stringKey);
	serial_print(eBuff);
#endif
}

void CapacitiveKey::release() {
	releaseKey(key);
	keyReleased = true;
#if defined(DEBUG)
	char eBuff[64];
	sprintf(eBuff, "[%20d] ", get_absolute_time());
	serial_print(eBuff);
	sprintf(eBuff, "Key %d Released %c\n", id, stringKey);
	serial_print(eBuff);
#endif
}
