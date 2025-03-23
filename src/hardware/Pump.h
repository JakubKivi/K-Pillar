#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
private:
    int pin;
    bool state;
public:
    int id;
    Pump(int pin, int id);
    void setState(bool newState);
    bool getState();
};

#endif
