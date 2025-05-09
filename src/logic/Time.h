#ifndef TIME_H
#define TIME_H

#include <stdint.h>

class Time {
public:
    uint8_t hour;
    uint8_t minute;

    Time(uint8_t h = 0, uint8_t m = 0);
    bool isLaterThan(const Time& other) const;
};

#endif
