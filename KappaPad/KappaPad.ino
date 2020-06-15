#include <CapacitiveSensor.h>
#include <Keyboard.h>

#include "CapacitiveKey.h"

#define SERIAL_OUTPUT
#define DISABLE_PIN 13

void setup() {
#ifdef SERIAL_OUTPUT
  Serial.begin(115200);
#endif
  Keyboard.begin();
  pinMode(DISABLE_PIN, INPUT_PULLUP);
}

CapacitiveKey keys[] = {
  CapacitiveKey(
    2,    //Capacitive Send Pin
    A5,    //Capacitive Sense Pin
    12,    //Capacitive Treshold
    'x'  //Keyboard Key
  ),

  CapacitiveKey(
    2,
    A4,
    12,
    'd'
  )
};

int keysLength = sizeof(keys) / sizeof(keys[0]);

void loop() {
  bool keyboardActive = digitalRead(DISABLE_PIN);
  for (int i = 0; i < keysLength; i++) {
    keys[i].keyUpdate(keyboardActive);
  }

#ifdef SERIAL_OUTPUT
  for (int i = 0; i < keysLength; i++) {
    if (keys[i].sample < 10) Serial.print ("  ");
    else if (keys[i].sample < 100) Serial.print(" ");

    if (i == keysLength - 1)
      Serial.println(String("Key#" + String(i) + ":" + keys[i].sample));
    else {
      Serial.print(String("Key#" + String(i) + ":" + keys[i].sample));
      Serial.print("   ");
    }
  }
#endif
}
