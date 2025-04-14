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

    MenuScreen currentScreen = PUMP1;
    MenuSubScreen currentSubScreen = ENABLE;    

    String inputBuffer = "";
    bool isSubmenu = false;
    bool isEditing = false;

    String formatTime(unsigned long milliseconds);
    String formatAmount(unsigned long time);
    String centerText(const char* text);
    String formatEditText(String text);

    void updateSchedule();

    void lcdDrawMenu();
    void lcdDrawSubMenu();
    void lcdDrawEditing();
    String fillEmpty(String input);

public:
    Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[]);
    void update(char key);
    void displayScreen();
    MenuScreen getCurrentScreen() const;
    
    void lcdCreateCustomCharacters();
    void lcdCreateHomeScreen();
};

#endif
