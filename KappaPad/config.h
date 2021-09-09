//Used mainly for debugging
// #define SERIAL_DEBUG

//Used to disable keypresses when SERIAL_DEBUG is defined
//Usefull for first setup
//#define DISABLE_KEYS

// Which pin to send HIGH or LOW depending on whetever keypad will send keys or not
// Use -1 if you don't want any LED for this
#define onOffLED 13

/*Threshold:
Threshold is how high the sample needs to be for it to count as a press.

Lets say that without touching the pad u get ~2
And pressing it you get ~6

The correct value would be something between 5 and 3,
This is to compensate any noise there may be in the samples
You don't want the key to unpress or press randomly, hopefully.

If the range is too low, like you get 2 not touching and 3 touching,
try to change the materials or length of wires/pads, this will impact
the sample.*/


/*Debounce:
This is how much time (in scanrate cycles) you want to pass to for the
key to be able to be released, if you set it to 0, the EXACT MOMENT
the sample is below the threshold EVEN ONCE will count as a release
you may want to set it as 0 is the press is WAY (more than 25 stable)
too high above the threshold otherwise try with increments of 5*/

//Key 0
#define key0SendPin 2
#define key0SensePin 3
#define key0Threshold 8
#define key0Debounce 50
#define key0char 'z'

//Key 1
#define key1SendPin 8
#define key1SensePin 12
#define key1Threshold 6
#define key1Debounce 150
#define key1char 'x'
