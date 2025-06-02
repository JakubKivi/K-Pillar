#ifndef RELAY_SCHEDULE_H
#define RELAY_SCHEDULE_H

#include "logic/Schedule.h"
#include "hardware/Relay.h"

class RelaySchedule : public Schedule {
private:
    bool isOn;
    unsigned long counterTime;
    TimeStruct TimeOn;
    TimeStruct TimeOff;
    TimeStruct LastSwitchTime;
    Relay* relay;

public:
    RelaySchedule(Relay* relay, Pump* pump, bool enabled, unsigned int intervalDays, 
                  TimeStruct wtrTime, unsigned long waterAmmount, 
                  EepromControl* EEPROM, LiquidCrystal_I2C* lcd,
                  TimeStruct TimeOn, TimeStruct TimeOff);
    
    bool update(TimeStruct currentTime, DateStruct currentDate) override;
    
    void setTimeOn(TimeStruct newTimeOn);
    void setTimeOff(TimeStruct newTimeOff);
    TimeStruct getTimeOn() const;
    TimeStruct getTimeOff() const;
};

#endif