#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "hardware/Pump.h"
#include "hardware/Relay.h"
#include "logic/Schedule.h"
#include "ui/Menu.h"
#include "power/PowerManager.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Pump pump1(A0, 1);
Pump pump2(A1, 2);
Pump pump3(A2, 3);
Relay relay(A3);

Schedule schedule1(&pump1, 0, 60000*5, 3000, &lcd);
Schedule schedule2(&pump2, 0, 60000*2, 2000, &lcd);
Schedule schedule3(&pump3, 0, 60000*13, 7000, &lcd);

Schedule* schedules[] = {&schedule1, &schedule2, &schedule3};
Menu menu(&lcd, &keypad, schedules);  

PowerManager powerManager(&lcd, &keypad);

void setup() {
    Serial.begin(9600);
    lcd.init();
    powerManager.wakeUp();
    menu.lcdCreateCustomCharacters();
    menu.lcdCreateHomeScreen();
    menu.displayScreen();
}

void loop() {
    // delay(INT32_MAX);
    powerManager.update();
    char key = keypad.getKey();
    if (key) {
        powerManager.resetTimer();  // Reset licznika usypiania
        
        menu.update(key);
    }

    if(schedule1.update() or schedule2.update() or schedule3.update()) 
        menu.displayScreen();
    delay(100);
}
