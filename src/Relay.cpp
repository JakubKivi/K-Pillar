#include "Relay.h"

Relay::Relay(int pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Relay::setState(bool newState) {
    state = newState;
    digitalWrite(pin, state ? HIGH : LOW);
}

bool Relay::getState() {
    return state;
}
