#ifndef EepromControl_h
#define EepromControl_h

#include <Wire.h>
#include <Arduino.h>
#include "logic/TimeStruct.h"

#define EEPROM_ADDR 0x50  // domyślny adres dla AT24C32

class EepromControl {
public:
    EepromControl();

    void Ewrite(unsigned int addr, byte data);
    byte Eread(unsigned int addr);

    void writeSchedule(uint8_t index, bool enabled, unsigned int intervalDays, const TimeStruct& time, unsigned long waterAmmount);
    void readAllSchedules(bool enabled[], unsigned int intervalDays[], TimeStruct times[], unsigned long waterAmmount[]);

private:
    unsigned int getScheduleBaseAddr(uint8_t index);
    void writeUInt16(unsigned int addr, uint16_t value);
    uint16_t readUInt16(unsigned int addr);
    void writeUInt32(unsigned int addr, uint32_t value);
    uint32_t readUInt32(unsigned int addr);
};

#endif
