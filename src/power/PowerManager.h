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
    unsigned long noInteractionThreshhold;
    
public:
    bool isSilentWakeUp = false;
    
    PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad,unsigned long noInteractionThreshhold);
    
    unsigned long getNoInteractionThreshhold();
    void setNoInteractionThreshhold(, bool updateEEPROM);

    void update();
    void resetTimer();
    void goToSleep();
    void wakeUp();
};

extern PowerManager powerManager;  // Globalny obiekt

#endif
