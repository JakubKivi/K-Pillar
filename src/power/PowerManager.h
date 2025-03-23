#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <Arduino.h>
#include <avr/sleep.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

class PowerManager {
private:
    LiquidCrystal_I2C* lcd;
    Keypad* keypad;
    unsigned long lastInteractionTime;
    bool isSleeping;
public:
    PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad);
    void update();
    void resetTimer();
    void goToSleep();
    void wakeUp();
    static void wakeUpISR();
};

extern PowerManager powerManager;  // Globalny obiekt

#endif
