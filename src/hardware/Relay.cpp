#include "Relay.h"

Relay::Relay(int pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Relay::setState(bool newState) {
    state = newState; // Stan zawsze się aktualizuje

#ifndef IS_DEV
    digitalWrite(pin, state ? HIGH : LOW);
#endif
}

bool Relay::getState() {
    return state;
}
