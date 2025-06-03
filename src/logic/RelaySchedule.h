#ifndef RELAY_SCHEDULE_H
#define RELAY_SCHEDULE_H

#include "logic/Schedule.h"
#include "hardware/Relay.h"

class RelaySchedule : public Schedule {
private:
    TimeStruct TimeOff;
    Relay* relay;

    void updateEEPROM() override;

public:
    RelaySchedule(Relay* relay, EepromControl* EEPROM, LiquidCrystal_I2C* lcd);
    
    bool update(TimeStruct currentTime, DateStruct currentDate) override;
    
    void setValues(bool enabled, TimeStruct wtrTime, TimeStruct timeOff);
    
    void setTimeOff(TimeStruct newTimeOff);
    void setTimeOff(String input);
    TimeStruct getTimeOff() const;
};

#endif