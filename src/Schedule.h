#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include "Pump.h"

class Schedule {
private:
    unsigned long lastWatered;
    unsigned long interval;
    Pump* pump;
public:
    Schedule(Pump* pump, unsigned long interval);
    void update();
    void setInterval(unsigned long newInterval);
    unsigned long getInterval();
};

#endif
