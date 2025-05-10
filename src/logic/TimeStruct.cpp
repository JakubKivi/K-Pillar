#include "TimeStruct.h"

TimeStruct::TimeStruct(uint8_t h, uint8_t m){
    if (h>23)
    {
        hour=23;
    }else{
        hour=h;
    }
    if (m>59)
    {
        minute=59;
    }else{
        minute=m;
    }
}

bool TimeStruct::isLaterThan(const TimeStruct& other) const {
    if (hour > other.hour) return true;
    if (hour < other.hour) return false;
    return minute > other.minute;
}
