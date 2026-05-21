#ifndef __LCD_TASK__
#define __LCD_TASK__

#include "model/global.h"
#include "kernel/Task.h"

class LCDTask : public Task {
    
private:
    void printPadded(int col, int row, String text, int totalWidth);
    State lastState;
    int lastPercent;

public:
    void init(int period);
    void tick();
};

#endif