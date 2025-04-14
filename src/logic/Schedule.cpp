#include "Schedule.h"

Schedule::Schedule(Pump* pump, bool enabled, unsigned long interval, unsigned long waterAmmount, LiquidCrystal_I2C* lcd)
    : pump(pump), enabled(enabled), interval(interval), waterAmmount(waterAmmount), lastWatered(0),  lcd(lcd) {}

bool Schedule::update() {

    if( enabled && waterAmmount > 0 ){
        if (millis() - lastWatered >= interval) {
            lcd->clear();
            lcd->setCursor(0, 0);
            String message = String(" Irrigating [")+ String(pump->id) + String("]");
            lcd->print(message);
            lcd->setCursor(0, 1);
            lcd->print("  Please wait..");
    
            pump->setState(true);
            delay(waterAmmount); // Podlewanie przez 5 sekund
            pump->setState(false);
            lastWatered = millis();

            return 1;
        }
    }else{
        lastWatered = millis();
    }
    return 0;
}

unsigned long Schedule::getAmmount(){
    return waterAmmount;
}

void Schedule::setAmmount(String ammount){
    
    waterAmmount= ammount.toInt();
}

void Schedule::setInterval(String newInterval) {
    interval = newInterval.substring(0,2).toInt() * 86400000  + newInterval.substring(2,4).toInt() * 3600000 + newInterval.substring(4,6).toInt() * 60000;
}

unsigned long Schedule::getInterval() {
    return interval;
}

bool Schedule::getEnabled(){
    return enabled;
}

void Schedule::setEnabled(bool input){
    enabled = input;
}