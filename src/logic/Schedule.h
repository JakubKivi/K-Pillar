#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "hardware/Pump.h"
#include "logic/TimeStruct.h"
#include <I2C_RTC.h>
#include "hardware/EepromControl.h"
#include "logic/DateStruct.h"

class Schedule {
private:
    static int globalIndexCounter; 
    int index; 

    Pump* pump;
    
    bool enabled;
    unsigned int intervalDays;
    TimeStruct wtrTime;
    unsigned long waterAmmount;
    
    DateStruct nextWatering;

    EepromControl* EEPROM;
    LiquidCrystal_I2C* lcd;

    void updateEEPROM();

public:

    void setValues(bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, DateStruct nextWatering);

    Schedule(Pump* pump, bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, EepromControl* EEPROM, LiquidCrystal_I2C* lcd);
    bool update(TimeStruct currentTime, DateStruct currentDate);

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
