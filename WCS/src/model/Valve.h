#ifndef __VALVE__
#define __VALVE__

#include "../devices/ServoMotor.h"

class Valve {

private:
  ServoMotor* servo;
  int openPercentage;
  bool initialized;

public:
  Valve(ServoMotor* servoMotor);
  void init();  // Call this from setup() after Serial is ready
  void open(int percentage);  // percentage: 0-100
  int getOpenPercentage();
};

#endif
