struct CapacitiveKey
{
	CapacitiveSensor *sensor;
	bool keyReleased = true;
	uint8_t key;
	char rawKey;

	unsigned int debounceDefault;
	unsigned int debounce;
	unsigned int threshold;
	long sample;
	uint8_t sendPin;
	uint8_t sensePin;
	CapacitiveKey(uint8_t _sendPin, uint8_t _sensePin, unsigned int capacitiveThreshold, unsigned int _debounce, char keyboardKey) {
		sendPin = _sendPin;
		sensePin = _sensePin;
		sensor = new CapacitiveSensor(_sendPin, _sensePin);

		threshold = capacitiveThreshold;

		debounceDefault = _debounce;

		rawKey = keyboardKey;
		key = keyboardKey - 93;

	}

	~CapacitiveKey() {
		delete sensor;
	}

#if defined(SERIAL_OUTPUT)
	void keyUpdate(bool kbEnabled) {
		sample = sensor->capacitiveSensorRaw();
		if(sample == -2)
			return;
#if !defined(ONLY_LOG)
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
#endif
	}
#else
	void keyUpdate() {
		sample = sensor->capacitiveSensorRaw();
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
#endif
};
