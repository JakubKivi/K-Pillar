#include "Menu.h"

Menu::Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[])
    : lcd(lcd), keypad(keypad), selectedPump(0) {
    for (int i = 0; i < 3; i++) {
        this->schedules[i] = schedules[i];
    }
}

void Menu::update() {
    char key = keypad->getKey();
    if (key) {
        if (key >= '1' && key <= '3') {
            selectedPump = key - '1';
        } else if (key == '#') {
            schedules[selectedPump]->setInterval(schedules[selectedPump]->getInterval() + 10000);
        }
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("Pump: ");
        lcd->print(selectedPump + 1);
        lcd->setCursor(0, 1);
        lcd->print("Interval: ");
        lcd->print(schedules[selectedPump]->getInterval() / 1000);
        lcd->print("s");
    }
}
