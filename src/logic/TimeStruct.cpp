#include "TimeStruct.h"

TimeStruct::TimeStruct(uint8_t h, uint8_t m) : hour(h), minute(m) {}

bool TimeStruct::isLaterThan(const TimeStruct& other) const {
    if (hour > other.hour) return true;
    if (hour < other.hour) return false;
    return minute > other.minute;
}
