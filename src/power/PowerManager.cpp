#include "PowerManager.h"


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
    if (millis() - lastInteractionTime > 5000) {
        // goToSleep(); 
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

    const byte ROWS = 4;
    const byte COLS = 3;
    byte rowPins[ROWS] = {2, 3, 4, 5};
    byte colPins[COLS] = {6, 7, 8};

    for (int i = 0; i < 4; i++) {
        pinMode(rowPins[i], INPUT_PULLUP);
      }

      for (int i = 0; i < 3; i++) {
        pinMode(colPins[i], OUTPUT);
        digitalWrite(colPins[i], LOW);
      }
    
    delay(1000);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    attachInterrupt(digitalPinToInterrupt(2), wakeUpISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), wakeUpISR, FALLING);

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

volatile bool PowerManager::wakeUpFlag = false;

void PowerManager::wakeUpISR() {
    PowerManager::wakeUpFlag = true;
}

