/*
First of all, comment the lines that say #define DEBUG and #define ENABLE_KEYS
Change the SendPin and SensePin based on your configuration, if you don't know what these
pins are, See the How it works section of the main README.md

Follow the instructions on how to build, the keypad will now start to detect your fingers
But not press any keys, Open the serial port of your keypad and check the values
that say K#S, this is the Sample value of the key, it will rise when you touch it,
Read Threshold info below

Once you have setted debounce try uncommenting the ENABLE_KEYS line, this will allow keypresses
play a bit and if you see repeated keys increase the debounce of the keyboard.

Read Debounce info below

Once you have a stable keyboard you may want to change the keys it presses for each key,
this doesn't really matter, as long as its a letter between lowercase a-z
Or change the key that toggles the keypad to be on or off

Threshold:
Threshold is how high the sample needs to be for it to count as a press.

Lets say that without touching the pad u get ~2
And pressing it you get ~6

The correct value would be something between 5 and 3,
This is to compensate any noise there may be in the samples
You don't want the key to unpress or press randomly, hopefully.

If the range is too low, like you get 2 not touching and 3 touching,
try to change the materials or length of wires/pads, this will impact
the sample.

Debounce:
This is how much time (in scanrate cycles) you want to pass to for the
key to be able to be released, if you set it to 0, the EXACT MOMENT
the sample is below the threshold EVEN ONCE will count as a release
you may want to set it as 0 is the press is WAY (more than 25 stable)
too high above the threshold otherwise try with increments of 5
*/

#define DEBUG
#define ENABLE_KEYS

// #define KEY_TOGGLE 0x1 // Num lock (https://wiki.osdev.org/USB_Human_Interface_Devices)
#define KEY_TOGGLE 0x2 // Caps lock (https://wiki.osdev.org/USB_Human_Interface_Devices)
// #define KEY_TOGGLE 0x3 // Caps lock OR Num lock (https://wiki.osdev.org/USB_Human_Interface_Devices)

// Which pin to send HIGH or LOW depending on whetever keypad will send keys or not
// Use -1 if you don't want any LED for this
#define onOffLED 13

// Key 0
#define key0SendPin   A0
#define key0SensePin  A1
#define key0Threshold 6
#define key0Debounce  50
#define key0char      'z'

// Key 1
#define key1SendPin   A5
#define key1SensePin  A4
#define key1Threshold 6
#define key1Debounce  50
#define key1char      'x'
