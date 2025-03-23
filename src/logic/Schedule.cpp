#include "Schedule.h"

Schedule::Schedule(Pump* pump, unsigned long interval, LiquidCrystal_I2C* lcd)
    : pump(pump), interval(interval), lastWatered(0), lcd(lcd) {}

void Schedule::update(int currentMenuScreen) {
    if (millis() - lastWatered >= interval) {
        lcd->clear();
        lcd->setCursor(0, 0);
        String message = String(" Irrigating [")+ String(pump->id) + String("]");
        lcd->print(message);
        lcd->setCursor(0, 1);
        lcd->print("  Please wait..");

        pump->setState(true);
        delay(5000); // Podlewanie przez 5 sekund
        pump->setState(false);


        lastWatered = millis();
    }
}

void Schedule::setInterval(unsigned long newInterval) {
    interval = newInterval;
}

unsigned long Schedule::getInterval() {
    return interval;
}
