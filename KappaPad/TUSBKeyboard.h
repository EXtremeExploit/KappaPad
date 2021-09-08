#include <Adafruit_TinyUSB.h>
#include <cstdint>

typedef uint8_t Keys[2];

class TinyKeyboard_ {
	private:
	void sendReport(Keys);

	public:
	TinyKeyboard_(void);
	void begin(void);
	void end(void);
	void press(uint8_t k);
	void release(uint8_t k);
};

extern TinyKeyboard_ Keyboard;
