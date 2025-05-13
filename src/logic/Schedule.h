#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"
#include "logic/TimeStruct.h"
#include <I2C_RTC.h>

class Schedule {
private:
    Pump* pump;
    
    bool enabled;
    unsigned int intervalDays;
    TimeStruct wtrTime;

    unsigned long waterAmmount;


    LiquidCrystal_I2C* lcd;
public:
    void setValues(bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount);

    bool wateredToday=0;
    Schedule(Pump* pump, bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, LiquidCrystal_I2C* lcd);
    bool update(TimeStruct currentTime);

    unsigned long getAmmount();
    void setAmmount(String ammount);

    void setInterval(String newInterval);
    unsigned long getInterval();

    TimeStruct getWtrTime();
    void setWtrTime(TimeStruct input);
    void setWtrTime(String input);

    bool getEnabled();
    void setEnabled(bool input);
};

#endif
