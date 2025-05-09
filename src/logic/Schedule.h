#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"
#include "logic/Time.h"

class Schedule {
private:
    Pump* pump;
    
    bool enabled;
    unsigned int intervalDays;
    Time irrigatingTime;

    unsigned long waterAmmount;
    unsigned long lastWatered;
    LiquidCrystal_I2C* lcd;
public:
    Schedule(Pump* pump, bool enabled, unsigned long interval, unsigned long waterAmmount, LiquidCrystal_I2C* lcd);
    bool update();

    unsigned long getAmmount();
    void setAmmount(String ammount);

    void setInterval(String newInterval);
    unsigned long getInterval();

    bool getEnabled();
    void setEnabled(bool input);
};

#endif
