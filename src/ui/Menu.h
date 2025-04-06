#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "logic/Schedule.h"
#include "power/PowerManager.h"

enum MenuScreen { PUMP1, PUMP2, PUMP3, RELAY, MANUAL, ABOUT };
enum MenuSubScreen { ENABLE, FREQ, AMMOUNT };

class Menu {
private:
    LiquidCrystal_I2C* lcd;
    Keypad* keypad;
    Schedule* schedules[3];

    MenuScreen currentScreen;
    MenuSubScreen currentSubScreen;    

    String inputBuffer;
    bool isSubmenu;
    bool isEditing;
public:
    Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[]);
    void update(char key);
    void displayScreen();
    MenuScreen getCurrentScreen() const;
};

#endif
