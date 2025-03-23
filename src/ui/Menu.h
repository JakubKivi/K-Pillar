#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "logic/Schedule.h"
#include "power/PowerManager.h"

enum MenuScreen { PUMP1, PUMP2, PUMP3, RELAY, ABOUT };

class Menu {
private:
    LiquidCrystal_I2C* lcd;
    Keypad* keypad;
    Schedule* schedules[3];
    PowerManager* powerManager;
    MenuScreen currentScreen;
    bool isEditing;
    String inputBuffer;
public:
    Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[], PowerManager* powerManager);
    void update();
    void displayScreen();
};

#endif
