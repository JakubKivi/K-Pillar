#include "Schedule.h"

Schedule::Schedule(Pump* pump, bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, LiquidCrystal_I2C* lcd)
    : pump(pump), enabled(enabled), intervalDays(intervalDays), wtrTime(wtrTime), waterAmmount(waterAmmount), lastWatered(0),  lcd(lcd) {}

bool Schedule::update(DS1307* RTC) {

    // RTC.

    if( enabled && waterAmmount > 0 ){
        if (false     ) {  //TODO warunek cały skonstruować
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
    intervalDays = newInterval.toInt();
}

unsigned long Schedule::getInterval() {
    return intervalDays;
}

bool Schedule::getEnabled(){
    return enabled;
}

void Schedule::setEnabled(bool input){
    enabled = input;
}

TimeStruct Schedule::getWtrTime(){
    return wtrTime;
}

void Schedule::setWtrTime(TimeStruct input){
    wtrTime = input;
}

void Schedule::setWtrTime(String input){

    wtrTime = TimeStruct( input.substring(0,2).toInt(), input.substring(2,4).toInt() );

    //wtrTime = input;
}