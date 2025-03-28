#include "Menu.h"
#include "utils/utils.h"

Menu::Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[], PowerManager* powerManager)
    : lcd(lcd), keypad(keypad), powerManager(powerManager), isEditing(false) {
    for (int i = 0; i < 3; i++) {
        this->schedules[i] = schedules[i];
    }
    currentScreen=PUMP1;
}

void Menu::update() {
    char key = keypad->getKey();
    if (key) {
        powerManager->resetTimer();  // Reset licznika usypiania

        if (isEditing) {
            if (key == '*') {  // Potwierdzenie edycji
                int days, hours;
                if (sscanf(inputBuffer.c_str(), "%d:%d", &days, &hours) == 2) {
                    schedules[currentScreen]->setInterval((days * 24 + hours) * 3600000UL);  // Zamiana na milisekundy
                }
                isEditing = false;
                inputBuffer = "";
            } else if (key == '#') {  // Anulowanie edycji
                isEditing = false;
                inputBuffer = "";
            } else if (isdigit(key) || key == ':') {  // Wpisywanie wartości
                inputBuffer += key;
            }
        } else {
            if (key == '4') {  // Lewo
                currentScreen = (currentScreen == PUMP1) ? ABOUT : (MenuScreen)(currentScreen - 1);
            } else if (key == '6') {  // Prawo
                currentScreen = (currentScreen == ABOUT) ? PUMP1 : (MenuScreen)(currentScreen + 1);
            } else if (key == '*') {  // Edycja interwału
                if (currentScreen != ABOUT) {
                    isEditing = true;
                    inputBuffer = "";
                }
            }
        }
        
        displayScreen();
    } 
}

void Menu::displayScreen() {
    lcd->clear();
    if(isEditing){
        switch (currentScreen) {
            case PUMP1:
            case PUMP2:
            case PUMP3:
                lcd->print("Set: ");
                lcd->print(inputBuffer);
                break;
            case RELAY:
                lcd->print("Set: ");
                lcd->print(inputBuffer);
                break;
            case ABOUT:
                lcd->setCursor(0, 0);
                lcd->print("   Kupa");
                lcd->setCursor(0, 1);
                lcd->print("   Dupa");
                break;
        }
    }else{
        lcdDrawMenu(lcd, currentScreen, schedules[currentScreen]->getInterval());
    }
    
}

MenuScreen Menu::getCurrentScreen() const{ 
    return currentScreen;
}