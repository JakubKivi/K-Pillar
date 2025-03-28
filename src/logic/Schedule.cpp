#include "Schedule.h"
#include "utils/utils.h"

Schedule::Schedule(Pump* pump, unsigned long interval, unsigned long water, LiquidCrystal_I2C* lcd)
    : pump(pump), interval(interval), lastWatered(0), water(water), lcd(lcd) {}

void Schedule::update(int currentScreen) {
    if(water>0){
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
    
    
            lcdDrawMenu(lcd, currentScreen, getInterval());
    
        }
    }
}

void Schedule::setInterval(unsigned long newInterval) {
    interval = newInterval;
}

unsigned long Schedule::getInterval() {
    return interval;
}
