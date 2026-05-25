#include "LCDTask.h"

void LCDTask::init(int period) {
    Task::init(period);
    lcd.init();
    lcd.backlight();
    lastState   = state;
    lastPercent = -1;
}

void LCDTask::printPadded(int col, int row, String text, int totalWidth) {
    lcd.setCursor(col, row);
    while ((int)text.length() < totalWidth) text += ' ';
    lcd.print(text);
}

void LCDTask::tick() {
    int currentPercent = valve->getOpenPercentage();
    if (state == lastState && currentPercent == lastPercent) return;
    lastState   = state;
    lastPercent = currentPercent;

    switch (state) {
        case MANUAL:        printPadded(0, 0, "Mode: MANUAL",        20); break;
        case AUTOMATIC:     printPadded(0, 0, "Mode: AUTOMATIC",     20); break;
        case REMOTE_MANUAL: printPadded(0, 0, "Mode: REMOTE MANUAL", 20); break;
        case UNCONNECTED: printPadded(0, 0, "Mode: UNCONNECTED", 20); break;
    }

    String valveStr = "Valve: " + String(currentPercent) + "%";
    printPadded(0, 1, valveStr, 20);
}