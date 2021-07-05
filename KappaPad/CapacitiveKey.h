struct CapacitiveKey
{
	CapacitiveSensor *sensor;
	bool keyReleased = true;
	uint8_t key;
	char rawKey;

	unsigned int releaseDelay = 50;
	unsigned int releaseTimer;
	unsigned int treshold;
	unsigned int sample;
	uint8_t sendPin;
	uint8_t sensePin;
	CapacitiveKey(uint8_t _sendPin, uint8_t _sensePin, unsigned int capacitiveTreshold, char keyboardKey) {
		sensor = new CapacitiveSensor(_sendPin, _sensePin);
		treshold = capacitiveTreshold;
		rawKey = keyboardKey;
		key = keyboardKey - 93;

		sendPin = _sendPin;
		sensePin = _sensePin;
	}

	~CapacitiveKey() {
		delete sensor;
	}

#if defined(SERIAL_OUTPUT)
	void keyUpdate(bool kbEnabled) {
		sample = sensor->capacitiveSensorRaw();
		if (sample > treshold) {
			if (keyReleased) {
				if (kbEnabled)
					Keyboard.press(key);
				keyReleased = false;
			}
			releaseTimer = releaseDelay;
		} else {
			if (!keyReleased) {
				if (releaseTimer == 0) {
					Keyboard.release(key);
					keyReleased = true;
				} else {
					releaseTimer--;
				}
			}
		}
	}
#else
	void keyUpdate() {
		sample = sensor->capacitiveSensorRaw();
		if (sample > treshold) {
			if (keyReleased) {
				Keyboard.press(key);
				keyReleased = false;
			}
			releaseTimer = releaseDelay;
		} else {
			if (!keyReleased) {
				if (releaseTimer == 0) {
					Keyboard.release(key);
					keyReleased = true;
				} else {
					releaseTimer--;
				}
			}
		}
	}
#endif
};
