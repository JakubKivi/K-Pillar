#include "PowerManager.h"
#include <avr/wdt.h>

volatile uint8_t wakeUpCounter = 0;
const uint8_t wakeUpThreshold = 8; // 23 * 8s ≈ 184s (~3 min)

volatile bool wakeUpFlag = false;
volatile bool isFirstWakeUp = true; 
volatile bool silentWakeUpFlag = false;

ISR(WDT_vect) {
    wakeUpCounter++;
    delay(100);
    if (wakeUpCounter >= wakeUpThreshold) {
        wakeUpCounter = 0; 
        isFirstWakeUp = true; 
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

bool PowerManager::update() {    
    if (wakeUpFlag) {
        wakeUp();
        wakeUpFlag = false;
        return true;
    }

    if (millis() - lastInteractionTime > noInteractionThreshhold) {
        goToSleep(); 
    }
    return false; // Nie było interakcji, nie aktualizujemy ekranu
}

void PowerManager::resetTimer() {
    lastInteractionTime = millis();
}

void globalWakeUpISR() {
    silentWakeUpFlag = false;
    if (isFirstWakeUp)
    {
        isFirstWakeUp = false;
    } else
    {
        isFirstWakeUp = true;
        wakeUpFlag = true;
    }
    
    delay(400);
}


void PowerManager::goToSleep() {
    delay(100);
    lcd->clear();
    lcd->noBacklight();

    pinMode(3, INPUT_PULLUP);
    pinMode(4, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);

    delay(400); // Daj czas na wyłączenie urządzeń 
    
    isSilentWakeUp = false;
    wakeUpFlag = false;

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

