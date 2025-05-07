#include "PowerManager.h"

PowerManager::PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad, int noInteractionThreshhold)
    : lcd(lcd), keypad(keypad), lastInteractionTime(0), noInteractionThreshhold(noInteractionThreshhold) {}

void PowerManager::update() {
    Serial.println("xd");
    
    if (wakeUpFlag) {
        Serial.println("1");
        wakeUp();
        wakeUpFlag = false;
        return;
    }

    if (millis() - lastInteractionTime > noInteractionThreshhold) {
        goToSleep(); 
        Serial.println("Sleeping");
    }
}

void PowerManager::resetTimer() {
    lastInteractionTime = millis();
}

void PowerManager::goToSleep() {
    lcd->noBacklight();
    
    Serial.println("Sleeping...");

    pinMode(3, INPUT_PULLUP);    
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);    
    
    attachInterrupt(digitalPinToInterrupt(3), PowerManager::wakeUpISR, FALLING);

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
    
    PowerManager::wakeUpFlag = false;
}

void PowerManager::wakeUp() {
    Serial.println("3");
    lcd->backlight();
    lastInteractionTime = millis();
    Serial.println("Waking up...");
}

volatile bool PowerManager::wakeUpFlag = false;

void PowerManager::wakeUpISR() {
    Serial.println("2");
    PowerManager::wakeUpFlag = true;
}

