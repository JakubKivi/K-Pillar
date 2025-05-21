#include "EepromControl.h"

#include <Arduino.h>
#include "logic/TimeStruct.h"

#define NUM_SCHEDULES 3

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
    return index * 10;  // przydziel 10 bajtów na każdy schedule
}

void EepromControl::writeSchedule(uint8_t index, bool enabled, unsigned int intervalDays, const TimeStruct& time, unsigned long waterAmmount, bool wateredToday, DateStruct nextWatering) {
    unsigned int base = getScheduleBaseAddr(index);
    Ewrite(base, enabled ? 1 : 0);
    writeUInt16(base + 1, intervalDays);
    Ewrite(base + 3, time.hour);
    Ewrite(base + 4, time.minute);
    writeUInt32(base + 5, waterAmmount);
    Ewrite(base+6, wateredToday? 1 : 0);
    Ewrite(base + 7, nextWatering.day);
    Ewrite(base + 8, nextWatering.month);
    Ewrite(base + 9, nextWatering.year);

}

void EepromControl::readAllSchedules(bool enabled[], unsigned int intervalDays[], TimeStruct times[], unsigned long waterAmmount[], bool wateredToday[], DateStruct nextWatering[]) {
    for (uint8_t i = 0; i < NUM_SCHEDULES; i++) {
        unsigned int base = getScheduleBaseAddr(i);
        enabled[i] = Eread(base) != 0;
        intervalDays[i] = readUInt16(base + 1);
        times[i].hour = Eread(base + 3);
        times[i].minute = Eread(base + 4);
        waterAmmount[i] = readUInt32(base + 5);
        wateredToday[i] = Eread(base+6) != 0;
        nextWatering[i].day = Eread(base+7);
        nextWatering[i].month = Eread(base+8);
        nextWatering[i].year = Eread(base+9);
    }
}
