#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "Pump.h"
#include "Relay.h"
#include "Schedule.h"
#include "Menu.h"

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

Pump pump1(A0);
Pump pump2(A1);
Pump pump3(A2);
Relay relay(A3);

Schedule schedule1(&pump1, 30000);
Schedule schedule2(&pump2, 45000);
Schedule schedule3(&pump3, 60000);

Schedule* schedules[] = {&schedule1, &schedule2, &schedule3};
Menu menu(&lcd, &keypad, schedules);

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void loop() {
    menu.update();
    schedule1.update();
    schedule2.update();
    schedule3.update();
}
