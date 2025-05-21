#ifndef DATESTRUCT_H
#define DATESTRUCT_H

#include <stdint.h>

class DateStruct {
public:
    uint8_t day;    // 1–31
    uint8_t month;  // 1–12
    uint16_t year;

    DateStruct();
    DateStruct(uint8_t d, uint8_t m, uint16_t y);

    // Różnica dni między this a other (może zwrócić ujemną wartość)
    int diffDays(const DateStruct& other) const;

private:
    bool isLeapYear(uint16_t y) const;
    int dayOfYear() const;
    int daysInYear(uint16_t y) const;
    long daysSinceEpoch() const; // liczba dni od 01.01.0000
};

#endif // DATESTRUCT_H
