#include "RelaySchedule.h"

RelaySchedule::RelaySchedule(Relay* relay, EepromControl* EEPROM, LiquidCrystal_I2C* lcd)
    : Schedule(nullptr, EEPROM, lcd),
      relay(relay){}

bool RelaySchedule::update(TimeStruct currentTime, DateStruct currentDate) {
    if (!enabled) {
        bool targetState = false;

        if (wtrTime.isLaterThan(TimeOff)) {// Normal
            targetState = !currentTime.isLaterThan(TimeOff) || 
                          currentTime.isLaterThan(wtrTime);

        } else {    // Through midnight
            targetState = currentTime.isLaterThan(wtrTime) && 
                        !currentTime.isLaterThan(TimeOff);
        }

        relay->setState(targetState);
    } else {
        relay->setState(false);
    }

    return false;
}

void RelaySchedule::setValues(bool enabled, TimeStruct wtrTime, TimeStruct timeOff) {
    this->enabled = enabled;
    this->wtrTime = wtrTime;
    this->TimeOff = timeOff;
    updateEEPROM();
}

void RelaySchedule::updateEEPROM(){
    EEPROM->writeRelaySchedule(enabled, wtrTime, TimeOff);
}

void RelaySchedule::setTimeOff(TimeStruct newTimeOff) {
    TimeOff = newTimeOff;
    updateEEPROM();
}

void RelaySchedule::setTimeOff(String input) {
    if (input.length() == 4) {
        TimeOff = TimeStruct(input.substring(0, 2).toInt(), input.substring(2, 4).toInt());
        updateEEPROM();
    }
}

TimeStruct RelaySchedule::getTimeOff() const {
    return TimeOff;
}