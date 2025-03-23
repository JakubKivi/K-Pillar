#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"

class Schedule {
private:
    unsigned long lastWatered;
    unsigned long interval;
    Pump* pump;
    LiquidCrystal_I2C* lcd;
public:
    Schedule(Pump* pump, unsigned long interval, LiquidCrystal_I2C* lcd);
    void update(int currentMenuScreen);
    void setInterval(unsigned long newInterval);
    unsigned long getInterval();
};

#endif
