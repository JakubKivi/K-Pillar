#include "PowerManager.h"

volatile bool wakeUpFlag = false;

PowerManager::PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad)
    : lcd(lcd), keypad(keypad), lastInteractionTime(0), isSleeping(false) {}

void PowerManager::update() {
    if (isSleeping) {
        if (wakeUpFlag) {
            wakeUp();
            wakeUpFlag = false;
        }
        return;
    }

    if (millis() - lastInteractionTime > 30000) {
        //goToSleep();
        Serial.println("Sleeping");
    }
}

void PowerManager::resetTimer() {
    lastInteractionTime = millis();
}

void PowerManager::goToSleep() {
    lcd->noBacklight();
    isSleeping = true;
    Serial.println("Sleeping...");

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    attachInterrupt(digitalPinToInterrupt(2), wakeUpISR, LOW);
    attachInterrupt(digitalPinToInterrupt(3), wakeUpISR, LOW);
    sleep_cpu();

    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(2));
    detachInterrupt(digitalPinToInterrupt(3));
}

void PowerManager::wakeUp() {
    lcd->backlight();
    isSleeping = false;
    lastInteractionTime = millis();
    Serial.println("Waking up...");
}

void PowerManager::wakeUpISR() {
    wakeUpFlag = true;
}

