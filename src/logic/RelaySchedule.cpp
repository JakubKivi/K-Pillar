#include "RelaySchedule.h"

RelaySchedule::RelaySchedule(Relay* relay, Pump* pump, bool enabled, unsigned int intervalDays,
                           TimeStruct wtrTime, unsigned long waterAmmount,
                           EepromControl* EEPROM, LiquidCrystal_I2C* lcd,
                           TimeStruct TimeOn, TimeStruct TimeOff)
    : Schedule(pump, enabled, intervalDays, wtrTime, waterAmmount, EEPROM, lcd),
      relay(relay), TimeOn(TimeOn), TimeOff(TimeOff), isOn(false), counterTime(0) {}

bool RelaySchedule::update(TimeStruct currentTime, DateStruct currentDate) {
    if (!enabled) {
        if (isOn) {
            relay->setState(false);
            isOn = false;
        }
        return true;
    }

    unsigned long currentTotalSeconds = currentTime.hour * 3600 + currentTime.minute* 60;
    unsigned long onTotalSeconds = TimeOn.hour * 3600 + TimeOn.minute * 60;
    unsigned long offTotalSeconds = TimeOff.hour * 3600 + TimeOff.minute * 60;

    if (currentTotalSeconds >= onTotalSeconds && currentTotalSeconds <= offTotalSeconds) {
        if (!isOn) {
            relay->setState(true);
            isOn = true;
            LastSwitchTime = currentTime;
        }
    } else {
        if (isOn) {
            relay->setState(false);
            isOn = false;
            LastSwitchTime = currentTime;
        }
    }

    return Schedule::update(currentTime, currentDate);
}

void RelaySchedule::setTimeOn(TimeStruct newTimeOn) {
    TimeOn = newTimeOn;
}

void RelaySchedule::setTimeOff(TimeStruct newTimeOff) {
    TimeOff = newTimeOff;
}

TimeStruct RelaySchedule::getTimeOn() const {
    return TimeOn;
}

TimeStruct RelaySchedule::getTimeOff() const {
    return TimeOff;
}