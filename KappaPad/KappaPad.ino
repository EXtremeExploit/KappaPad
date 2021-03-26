#include <CapacitiveSensor.h>
#include "Keyboard.h"

#include "CapacitiveKey.h"

//#define SERIAL_OUTPUT

void setup() {
#ifdef SERIAL_OUTPUT
  Serial.begin(115200);
#endif
  Keyboard.begin();
}

CapacitiveKey key0 = {
  
    3,    //Capacitive Send Pin
    A2,    //Capacitive Sense Pin
    5,    //Capacitive Treshold
    'x'  //Keyboard Key
};

CapacitiveKey key1 = {
    2,
    A0,
    6,
    'd'
};

void loop() {
  key0.keyUpdate();
  key1.keyUpdate();

#ifdef SERIAL_OUTPUT
  Serial.println(String(key0.sample) + "-" + String(key1.sample));
  // for (int i = 0; i < keysLength; i++) {
  //   if (keys[i].sample < 10) Serial.print ("  ");
  //   else if (keys[i].sample < 100) Serial.print(" ");

  //   if (i == keysLength - 1)
  //     Serial.println(String("Key#" + String(i) + ":" + keys[i].sample));
  //   else {
  //     Serial.print(String("Key#" + String(i) + ":" + keys[i].sample));
  //     Serial.print("   ");
  //   }
  // }
#endif
}
