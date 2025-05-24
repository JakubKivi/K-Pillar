#include "DateStruct.h"
#include <stdlib.h>

DateStruct::DateStruct()
    : day(1), month(1), year(2000) {}

DateStruct::DateStruct(uint8_t d, uint8_t m, uint16_t y){
    if (y < 1900) y = 1900;
    else if (y > 2100) y = 2100;
    year = y;

    // Clamp month to [1, 12]
    if (m < 1) m = 1;
    else if (m > 12) m = 12;
    month = m;

    // Determine max day in given month/year
    static const uint8_t daysInMonth[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    uint8_t maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year))
        maxDay = 29;

    // Clamp day to [1, maxDay]
    if (d < 1) d = 1;
    else if (d > maxDay) d = maxDay;
    day = d;
}

bool DateStruct::isLeapYear(uint16_t y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int DateStruct::daysInYear(uint16_t y) const {
    return isLeapYear(y) ? 366 : 365;
}

int DateStruct::dayOfYear() const {
    static const uint16_t daysBeforeMonth[12] = {
        0,   31,  59,  90, 120, 151,
        181, 212, 243, 273, 304, 334
    };

    if (month < 1 || month > 12 || day < 1 || day > 31)
        return -1;

    int dayNum = daysBeforeMonth[month - 1] + day;

    if (isLeapYear(year) && month > 2)
        dayNum += 1;

    return dayNum;
}

// Liczba dni od 01.01.0000 do tej daty
long DateStruct::daysSinceEpoch() const {
    long days = 0;
    for (uint16_t y = 0; y < year; ++y) {
        days += daysInYear(y);
    }
    days += dayOfYear();
    return days;
}


int DateStruct::diffDays(const DateStruct& other) const {
    return (int)(this->daysSinceEpoch() - other.daysSinceEpoch());
}

void DateStruct::addDays(int daysToAdd) {
    long totalDays = daysSinceEpoch() + daysToAdd;

    // Now convert totalDays back to year, month, day
    uint16_t newYear = 0;
    while (totalDays >= daysInYear(newYear)) {
        totalDays -= daysInYear(newYear);
        ++newYear;
    }

    static const uint8_t daysInMonth[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    uint8_t newMonth = 1;
    while (true) {
        uint8_t dim = daysInMonth[newMonth - 1];
        if (newMonth == 2 && isLeapYear(newYear))
            dim = 29;

        if (totalDays < dim)
            break;

        totalDays -= dim;
        ++newMonth;
    }

    day = (uint8_t)(totalDays + 1);
    month = newMonth;
    year = newYear;
}

DateStruct& DateStruct::operator+=(int daysToAdd) {
    addDays(daysToAdd);
    return *this;
}