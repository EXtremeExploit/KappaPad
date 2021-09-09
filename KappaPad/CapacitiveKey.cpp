#include "CapacitiveKey.h"

CapacitiveKey::CapacitiveKey(uint8_t _sendPin,
							 uint8_t _sensePin,
							 unsigned int capacitiveThreshold,
							 unsigned int _debounce,
							 char keyboardKey) {
	sendPin  = _sendPin;
	sensePin = _sensePin;
	sensor   = new CapacitiveSensor(_sendPin, _sensePin);

	threshold = capacitiveThreshold;

	debounceDefault = _debounce;

	rawKey = keyboardKey;
	key    = keyboardKey - 93;
}

CapacitiveKey::~CapacitiveKey() {
	delete sensor;
}

void CapacitiveKey::keyUpdate(bool kbEnabled) {
	sample = sensor->SenseOneCycle();
	if (sample < 0)
		return;

#if !defined(DISABLE_KEYS)
	if (kbEnabled)
		if (sample > threshold) {
			if (keyReleased) {
				Keyboard.press(key);
				keyReleased = false;
			}
			debounce = debounceDefault;
		} else {
			if (!keyReleased) {
				if (debounce == 0) {
					Keyboard.release(key);
					keyReleased = true;
				} else {
					debounce--;
				}
			}
		}
#endif // !defined(DISABLE_KEYS)
}
void CapacitiveKey::keyUpdate() {
	sample = sensor->SenseOneCycle();
	if (sample > threshold) {
		if (keyReleased) {
			Keyboard.press(key);
			keyReleased = false;
		}
		debounce = debounceDefault;
	} else {
		if (!keyReleased) {
			if (debounce == 0) {
				Keyboard.release(key);
				keyReleased = true;
			} else {
				debounce--;
			}
		}
	}
}