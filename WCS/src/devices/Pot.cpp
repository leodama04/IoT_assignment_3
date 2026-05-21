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
    return (int)(value / 1023.0 * 100);
}

void Potentiometer::updateSyncTime(long time){
	lastTimeSync = time;
}

long Potentiometer::getLastSyncTime(){
	return lastTimeSync;
}