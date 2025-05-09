#include "Time.h"

Time::Time(uint8_t h, uint8_t m) : hour(h), minute(m) {}

bool Time::isLaterThan(const Time& other) const {
    if (hour > other.hour) return true;
    if (hour < other.hour) return false;
    return minute > other.minute;
}
