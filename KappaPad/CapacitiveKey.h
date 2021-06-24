struct CapacitiveKey
{
	CapacitiveSensor *sensor;
	bool keyReleased = true;
	char key;

	unsigned int releaseDelay = 50;
	unsigned int releaseTimer;
	unsigned int treshold;
	unsigned int sample;
	CapacitiveKey(uint8_t sendPin, uint8_t receivePin, unsigned int capacitiveTreshold, char keyboardKey) {
		sensor = new CapacitiveSensor(sendPin, receivePin);
		treshold = capacitiveTreshold;
		key = keyboardKey;
	}

	~CapacitiveKey() {
		delete sensor;
	}
	void keyUpdate(bool kbEnabled) {
#ifdef SERIAL_OUTPUT
		sample = sensor->capacitiveSensorRaw();
		if (sample > treshold && kbEnabled) {
			if (keyReleased && kbEnabled) {
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
#endif
#ifndef SERIAL_OUTPUT
		if(kbEnabled) {
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
		}else{
			if(!keyReleased) {
				Keyboard.release(key);
				keyReleased = true;
			}
		}
#endif
	}
};
