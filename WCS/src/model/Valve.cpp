#include "Valve.h"
#include "Arduino.h"

Valve::Valve(ServoMotor* servoMotor) : servo(servoMotor), openPercentage(0), initialized(false) { }

void Valve::init() {
  if (!initialized) {
    servo->on();
    initialized = true;
  }
}

void Valve::open(int percentage) {
  if (!initialized) {
    return;
  }
  this->openPercentage = percentage;
  int angle = (openPercentage * 90) / 100;
  servo->setPosition(angle);
}

int Valve::getOpenPercentage() {
  return this->openPercentage;
}
