#include "EepromControl.h"

#include <Arduino.h>
#include "logic/TimeStruct.h"

EepromControl::EepromControl() {
    Wire.begin();
}

void EepromControl::Ewrite(unsigned int addr, byte data) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(addr >> 8));
    Wire.write((int)(addr & 0xFF));
    Wire.write(data);
    Wire.endTransmission();
    delay(5);
}

byte EepromControl::Eread(unsigned int addr) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(addr >> 8));
    Wire.write((int)(addr & 0xFF));
    Wire.endTransmission();

    Wire.requestFrom(EEPROM_ADDR, 1);
    if (Wire.available()) return Wire.read();
    return 0xFF;
}

void EepromControl::writeUInt16(unsigned int addr, uint16_t value) {
    Ewrite(addr, value >> 8);
    Ewrite(addr + 1, value & 0xFF);
}

uint16_t EepromControl::readUInt16(unsigned int addr) {
    return (Eread(addr) << 8) | Eread(addr + 1);
}

void EepromControl::writeUInt32(unsigned int addr, uint32_t value) {
    for (int i = 0; i < 4; i++) {
        Ewrite(addr + i, (value >> (8 * (3 - i))) & 0xFF);
    }
}

uint32_t EepromControl::readUInt32(unsigned int addr) {
    uint32_t val = 0;
    for (int i = 0; i < 4; i++) {
        val <<= 8;
        val |= Eread(addr + i);
    }
    return val;
}

unsigned int EepromControl::getScheduleBaseAddr(uint8_t index) {
    return index * 16;  
}

void EepromControl::writeSchedule(uint8_t index, bool enabled, unsigned int intervalDays, const TimeStruct& time, unsigned long waterAmmount, DateStruct nextWatering) {
    unsigned int base = getScheduleBaseAddr(index);
    Ewrite(base, enabled ? 1 : 0);
    writeUInt16(base + 1, intervalDays);
    Ewrite(base + 3, time.hour);
    Ewrite(base + 4, time.minute);
    Ewrite(base + 5, nextWatering.day);
    Ewrite(base + 6, nextWatering.month);
    writeUInt16(base + 7, nextWatering.year);
    writeUInt32(base + 9, waterAmmount);

}

void EepromControl::writeRelaySchedule( bool enabled, TimeStruct time, TimeStruct timeOff){
    unsigned int base = getScheduleBaseAddr(NUM_SCHEDULES+1);

    Ewrite(base, enabled ? 1 : 0);
    Ewrite(base + 1, time.hour);
    Ewrite(base + 2, time.minute);
    Ewrite(base + 3, timeOff.hour);
    Ewrite(base + 4, timeOff.minute);
}

void EepromControl::readAllSchedules(bool enabled[], unsigned int intervalDays[], TimeStruct times[], unsigned long waterAmmount[], DateStruct nextWatering[]) {
    for (uint8_t i = 0; i < NUM_SCHEDULES; i++) {
        unsigned int base = getScheduleBaseAddr(i);
        enabled[i] = Eread(base) != 0;
        intervalDays[i] = readUInt16(base + 1);
        times[i].hour = Eread(base + 3);
        times[i].minute = Eread(base + 4);
        nextWatering[i].day = Eread(base+5);
        nextWatering[i].month = Eread(base+6);
        nextWatering[i].year = readUInt16(base+7);
        waterAmmount[i] = readUInt32(base + 9);
    }
}

void EepromControl::readRelaySchedule(bool *enabled, TimeStruct *time, TimeStruct *timeOff){
    unsigned int base = getScheduleBaseAddr(NUM_SCHEDULES+1);

    *enabled = Eread(base); 
    time->hour = Eread(base + 1);
    time->minute = Eread(base + 2);
    timeOff->hour = Eread(base + 3);
    timeOff->minute = Eread(base + 4);
}


void EepromControl::saveSettings(uint16_t setting1) {
    writeUInt16(SETTINGS_BASE_ADDR, setting1);   
}

void EepromControl::readSettings(uint16_t& setting1) {
    setting1 = readUInt16(SETTINGS_BASE_ADDR);
}
