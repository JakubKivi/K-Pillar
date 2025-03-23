#include "Pump.h"

#define IS_DEV  // Tryb deweloperski bez zmiany pinów (zakomentuj, żeby wyłączyć)

Pump::Pump(int pin, int id) : pin(pin), state(false), id(id) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Pump::setState(bool newState) {
    state = newState; // Stan zawsze się aktualizuje

#ifndef IS_DEV
    digitalWrite(pin, state ? HIGH : LOW);
#endif
}

bool Pump::getState() {
    return state;
}
