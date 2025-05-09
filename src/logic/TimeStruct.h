#ifndef TIMESTRUCT_H
#define TIMESTRUCT_H

#include <stdint.h>

class TimeStruct {
public:
    uint8_t hour;
    uint8_t minute;

    TimeStruct(uint8_t h = 0, uint8_t m = 0);
    bool isLaterThan(const TimeStruct& other) const;
};

#endif
