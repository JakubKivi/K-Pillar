#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
private:
    int pin;
    bool state;
public:
    Pump(int pin);
    void setState(bool newState);
    bool getState();
};

#endif
