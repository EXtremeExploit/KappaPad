class CapacitiveKey
{
public:
  CapacitiveSensor *sensor;
  bool keyReleased = true;
  char key;

  unsigned int releaseDelay = 20;
  unsigned int releaseTimer;
  unsigned int treshold;
  unsigned int sample;
  CapacitiveKey(uint8_t sendPin,
                uint8_t receivePin,
                unsigned int capacitiveTreshold,
                char keyboardKey)
  {
    sensor = new CapacitiveSensor(sendPin, receivePin);
    treshold = capacitiveTreshold;
    key = keyboardKey;
  }
  ~CapacitiveKey()
  {
    delete sensor;
  }
  void keyUpdate(bool keyboardActive)
  {
    sample = sensor->capacitiveSensorRaw(1);
    if (sample > treshold)
    {
      if (keyReleased)
      {
        if (keyboardActive)
          Keyboard.press(key);
        keyReleased = false;
      }
      releaseTimer = releaseDelay;
    }
    else
    {
      if (!keyReleased)
      {
        if (releaseTimer == 0)
        {
          Keyboard.release(key);
          keyReleased = true;
        }
        else
        {
          releaseTimer--;
        }
      }
    }
  }
};
