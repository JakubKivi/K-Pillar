#include "PowerManager.h"
#include <avr/wdt.h>

volatile uint8_t wakeUpCounter = 0;
const uint8_t wakeUpThreshold = 8; // 23 * 8s ≈ 184s (~3 min)

volatile bool wakeUpFlag = false;
volatile bool silentWakeUpFlag = false;

ISR(WDT_vect) {
    wakeUpCounter++;
    if (wakeUpCounter >= wakeUpThreshold) {
        wakeUpFlag = true;
        silentWakeUpFlag = true;
    }
}

PowerManager::PowerManager(LiquidCrystal_I2C* lcd, Keypad* keypad, unsigned long noInteractionThreshhold)
    : lcd(lcd), keypad(keypad), lastInteractionTime(0), noInteractionThreshhold(noInteractionThreshhold) {
        sleep_disable();
        detachInterrupt(digitalPinToInterrupt(3));
        MCUSR &= ~(1 << WDRF); // Clear watchdog reset flag
        wdt_disable();         // Ensure watchdog is fully disabled at startup
}

unsigned long PowerManager::getNoInteractionThreshhold(){
    return noInteractionThreshhold;
}


void PowerManager::setNoInteractionThreshhold(unsigned long input){
    noInteractionThreshhold = input;
}

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
    silentWakeUpFlag = false;
    wakeUpFlag = true;
}


void PowerManager::goToSleep() {
    // Serial.println("Ide w spanko");
    delay(100);

    lcd->noBacklight();

    pinMode(3, INPUT_PULLUP);    
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(12, OUTPUT);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);    
    
    wakeUpCounter = 0;
    isSilentWakeUp = false;

    // Int pin 3
    attachInterrupt(digitalPinToInterrupt(3), globalWakeUpISR, FALLING);

    // Watchdog each 8s
    MCUSR &= ~(1 << WDRF); // Clear WDRF
    WDTCSR |= (1 << WDCE) | (1 << WDE); // WDT change enable
    WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Interrupt enable, timeout = 8s

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    while (!wakeUpFlag) {
        sleep_cpu(); // śpij, aż przerwanie coś zrobi
    }

    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(3));

    // Wyłącz watchdog po przebudzeniu
    wdt_disable();
    
    pinMode(3, OUTPUT);    
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);    
}

void PowerManager::wakeUp() {
    if (silentWakeUpFlag)
    {
        isSilentWakeUp=true;
        // Serial.println("Cicha pobudka");
    }else{
        isSilentWakeUp=false;
        lcd->backlight();
    }
    
    lastInteractionTime = millis();
}

