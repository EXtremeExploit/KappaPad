# KappaPad

KappaPad is a two-button keypad intended for playing osu!. Unlike other keypads with mechanical switches, KappaPad has fixed metal pads and uses capacitive sensing to determine when they are being pressed. Because there is no key travel, one can minimize the distance their fingers move to conserve stamina during long streams.

![KappaPad](images/full.jpg?raw=true)

## Usage

Just plug the KappaPad into a computer, and it should work as a USB keyboard, with the keys bound to Z and X. (The key bindings can be changed in the Arduino sketch.)

## How to Make One

There aren't any plans to sell these yet, but it is fairly simple to build your own. Here is an example of how to build one, but you don't have to follow it exactly. Just use the materials you have on hand.

### Physical Construction

The base should ideally be heavy and have a non-slip surface on the bottom. The keypad pictured has a base made out of 3/8 inch (~1cm) acrylic, with rubber feet on the bottom. Components were attached with double-sided foam tape.

### Electronics

![Closeup of Electronics](images/electronics.jpg?raw=true)

Components Used

* 170 Point Breadboard
* Clone of Arduino Pro Micro
* 2x 100 Ohm Resistor
* 1x 220 Ohm Resistor
* 2x 330 kOhm Resistor
* Solid-Core Wire
* Common-Cathode RGB LED

All of the electronics were connected by a small breadboard. An [Arduino Pro Micro](https://www.sparkfun.com/products/12640)-compatible board was used as the microcontroller. Other boards that can act as a USB keyboard can be used, such as Arduino Leonardo, Arduino Zero, Arduino Due, and Teensy. The optional LED will light up red or green depending on which key was pressed. It was mounted pointing downwards in a hole in the acrylic base to provide illumination all around the edges.

The keys were made out of sheet aluminum, with wires crimped to provide a secure electrical connection. Any metal objects can be used for the keys. The size and material of the objects will determine the treshold values when calibrating the keyboard.

### Software

The Arduino Sketch is located in the KappaPad directory of this repository. It requires the [CapacitiveSensor library](https://github.com/PaulStoffregen/CapacitiveSensor) so install that first from: Sketch > Import Library > Manage Libraries. 

Change the PINS you will use in KappaPad.ino.

* **IF YOU ARE GONNA CHANGE KEYS, ONLY LOWERCASE ENGLISH ALPHABET ARE SUPPORTED FOR OPTIMIZATIONS (a-z)**

Upload the sketch to the board using the appropriate board type. (Use Arduino Leonardo if you have a Pro Micro.) After uploading, note that your board may recieve a different serial port number.

At this point the keyboard may work, or the treshold values may have to be adjusted. If keys act like they are being held down and your computer is now unusable when the keypad is plugged in, Comment the line that says `Keyboard.press(key);` inside `CapacitiveKey.h`  and enable serial output by uncommenting `SERIAL_OUTPUT` in `KappaPad.ino`, that will show you the threshold the sensor is now, put it 1+ to the highest value you see when not touching the pads, disable serial output again to improve performance.
