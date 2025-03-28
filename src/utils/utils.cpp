#include "utils.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

String formatTime(unsigned long milliseconds) {
    unsigned long totalMinutes = milliseconds / 60000;  // Konwersja na minuty
    unsigned long minutes = totalMinutes % 60;
    unsigned long totalHours = totalMinutes / 60;
    unsigned long hours = totalHours % 24;
    unsigned long days = totalHours / 24;

    return String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
}

void lcdDrawMenu(LiquidCrystal_I2C *lcd, int currentScreen, unsigned long interval){
    lcd->clear();
        switch (currentScreen) {
            case 1:
            case 2:
            case 3:
                lcd->setCursor(0, 0);
                lcd->print("Pump ");
                lcd->print(currentScreen + 1);
                lcd->setCursor(0, 1);
                lcd->print(formatTime(interval));
                break;
            case 4:
                lcd->setCursor(0, 0);
                lcd->print("Relay");
                lcd->setCursor(0, 1);
                lcd->print(formatTime(interval));
                break;
            case 5:
                lcd->setCursor(0, 0);
                lcd->print("   Created by");
                lcd->setCursor(0, 1);
                lcd->print("   JakubKivi");
                break;
        }
}