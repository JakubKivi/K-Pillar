#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
private:
    int pin;
    bool state;
public:
    Relay(int pin);
    void setState(bool newState);
    bool getState();
};

#endif
