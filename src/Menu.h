#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "Schedule.h"

class Menu {
private:
    LiquidCrystal_I2C* lcd;
    Keypad* keypad;
    Schedule* schedules[3];
    int selectedPump;
public:
    Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[]);
    void update();
};

#endif
