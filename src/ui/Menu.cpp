#include "Menu.h"
#include "utils/utils.h"

Menu::Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[])
    : lcd(lcd), keypad(keypad), isEditing(false) {
    for (int i = 0; i < 3; i++) {
        this->schedules[i] = schedules[i];
    }
    currentScreen=PUMP1;
    currentSubScreen=ENABLE;
    isSubmenu=false;
}

void Menu::update(char key) {
    if (key) {
        if(!isSubmenu && !isEditing){
            if (key == '4') {  // Lewo
                currentScreen = (currentScreen == PUMP1) ? ABOUT : (MenuScreen)(currentScreen - 1);
            } else if (key == '6') {  // Prawo
                currentScreen = (currentScreen == ABOUT) ? PUMP1 : (MenuScreen)(currentScreen + 1);
            } else if (key == '*') {  // Otwarcie submenu
                if (currentScreen != ABOUT) {
                    isSubmenu = true;
                }
            }
        }else if(isSubmenu){
            if (key == '4') {  // Lewo
                currentSubScreen = (currentSubScreen == ENABLE) ? AMMOUNT : (MenuSubScreen)(currentSubScreen - 1);
            } else if (key == '6') {  // Prawo
                currentSubScreen = (currentSubScreen == AMMOUNT) ? ENABLE : (MenuSubScreen)(currentSubScreen + 1);
            }else if (key == '#') {  // Otwarcie submenu
                isSubmenu = false;
            } else if (key == '*') {  // Otwarcie submenu
                if (currentSubScreen != ENABLE) {
                    isEditing = true;
                    isSubmenu = false;
                }else{
                    schedules[currentScreen]->setEnabled(!(schedules[currentScreen]->getEnabled()));
                }
            }
        }else{
            if (key == '#') {  // Otwarcie submenu
                isSubmenu = true;
                isEditing=false;
            }
        }
        displayScreen();
    } 
}

void Menu::displayScreen() {
    lcd->clear();
    if (isSubmenu)
    {
        lcdDrawSubMenu(lcd, currentSubScreen, schedules[currentScreen]);
    }else if (isEditing)
    {
        lcdDrawEditing(lcd);
    }else{
        lcdDrawMenu(lcd, currentScreen, schedules[currentScreen]->getInterval());    
    }
}

MenuScreen Menu::getCurrentScreen() const{ 
    return currentScreen;
}