#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "logic/Schedule.h"
#include "power/PowerManager.h"
#include "logic/TimeStruct.h"
#include "logic/TimeStruct.h"

enum MenuScreen { PUMP1, PUMP2, PUMP3, RELAY, SETTINGS};
enum MenuSubScreen { ENABLE, FREQ, TIMING, AMMOUNT, NEXT };

enum MenuSettingsScreen {TIME, SLEEPING, ABOUT};

class Menu {
private:
    LiquidCrystal_I2C* lcd;
    Keypad* keypad;
    Schedule* schedules[3];
    DS1307* RTC;

    MenuScreen currentScreen = PUMP1;
    MenuSubScreen currentSubScreen = ENABLE;    
    MenuSettingsScreen currentMenuSettingsScreen = TIME;

    TimeStruct currentTime;

    String inputBuffer = "";
    bool isSubmenu = false;
    bool isEditing = false;

    String formatTime(TimeStruct time);
    String formatEditTime(String input);
    String formatAmount(unsigned long ammount);
    String centerText(const char* text);

    void updateSchedule();

    void lcdDrawMenu();
    void lcdDrawSubMenu();
    void lcdDrawEditing();
    String fillEmpty(String input);

public:
    Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[], DS1307* RTC);
    void update(char key);
    void displayScreen();
    MenuScreen getCurrentScreen() const;

    TimeStruct getCurrentTime();
    void setCurrentTime(TimeStruct input, bool updateRTC);
    
    void lcdCreateCustomCharacters();
    void lcdCreateHomeScreen();
};

#endif
