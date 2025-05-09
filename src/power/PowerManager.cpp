#include "PowerManager.h"

volatile bool wakeUpFlag = false;

PowerManager::PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad, unsigned long noInteractionThreshhold)
    : lcd(lcd), keypad(keypad), lastInteractionTime(0), noInteractionThreshhold(noInteractionThreshhold) {}

void PowerManager::update() {    
    if (wakeUpFlag) {
        wakeUp();
        wakeUpFlag = false;
        return;
    }

    if (millis() - lastInteractionTime > noInteractionThreshhold) {
        goToSleep(); 
    }
}

void PowerManager::resetTimer() {
    lastInteractionTime = millis();
}

void globalWakeUpISR() {
    wakeUpFlag = true;
}

void PowerManager::goToSleep() {
    lcd->noBacklight();

    pinMode(3, INPUT_PULLUP);    
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);    
    
    attachInterrupt(digitalPinToInterrupt(3), globalWakeUpISR, FALLING);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
    // -- tu MCU śpi, aż przyjdzie FALLING na pinie 3 --
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(3));
    
    pinMode(3, OUTPUT);    
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);    
}

void PowerManager::wakeUp() {
    lcd->backlight();
    lastInteractionTime = millis();
}

