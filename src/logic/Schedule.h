#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"
#include "logic/TimeStruct.h"
#include <I2C_RTC.h>
#include "hardware/EepromControl.h"
#include "logic/DateStruct.h"

class Schedule
{
private:
    static int globalIndexCounter;
    int index;

    Pump *pump;

    unsigned int intervalDays;
    unsigned long waterAmmount;

    DateStruct nextWatering;

    LiquidCrystal_I2C *lcd;

    virtual void updateEEPROM();

protected:
    bool enabled;

    EepromControl *EEPROM;
    TimeStruct wtrTime;

public:
    void setValues(bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, DateStruct nextWatering);

    Schedule(Pump *pump, EepromControl *EEPROM, LiquidCrystal_I2C *lcd);
    virtual bool update(TimeStruct currentTime, DateStruct currentDate);

    unsigned long getAmmount();
    void setAmmount(String ammount);

    void setInterval(String newInterval);
    unsigned long getInterval();

    TimeStruct getWtrTime();
    void setWtrTime(TimeStruct input);
    void setWtrTime(String input);

    DateStruct getNextWatering();
    void setNextWatering(DateStruct nextWatering);

    bool getEnabled();
    void setEnabled(bool input);
};

#endif
