#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"

class Schedule {
private:
    Pump* pump;
    unsigned long interval;
    unsigned long water;
    unsigned long lastWatered;
    LiquidCrystal_I2C* lcd;
public:
    Schedule(Pump* pump, unsigned long interval, unsigned long water, LiquidCrystal_I2C* lcd);
    void update(int currentMenuScreen);
    void setInterval(unsigned long newInterval);
    unsigned long getInterval();
};

#endif
