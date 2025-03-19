#include "Pump.h"

Pump::Pump(int pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Pump::setState(bool newState) {
    state = newState;
    digitalWrite(pin, state ? HIGH : LOW);
}

bool Pump::getState() {
    return state;
}
