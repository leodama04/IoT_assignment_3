#ifndef __TIMER__
#define __TIMER__

extern volatile bool timerFlag;

class Timer {
    
public:  
  Timer();
  void setupFreq(int freq);  
  /* period in ms */
  void setupPeriod(int period);  
  void waitForNextTick();

};


#endif