#include "Pot.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
} 
  
void Potentiometer::sync(){
  value = analogRead(pin);  
  updateSyncTime(millis());
}

int Potentiometer::getValue(){
  int percent = (int)(value / 1023.0 * 100);
  return (value >= 1015) ? 100 : percent;
}

void Potentiometer::updateSyncTime(long time){
	lastTimeSync = time;
}

long Potentiometer::getLastSyncTime(){
	return lastTimeSync;
}