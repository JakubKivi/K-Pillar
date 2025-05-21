#include "Schedule.h"
#include "hardware/EepromControl.h"

int Schedule::globalIndexCounter = 0;


Schedule::Schedule(Pump* pump, bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, EepromControl* EEPROM,  LiquidCrystal_I2C* lcd)
    : pump(pump), enabled(enabled), intervalDays(intervalDays), wtrTime(wtrTime), waterAmmount(waterAmmount), EEPROM(EEPROM), lcd(lcd) {}

void Schedule::setValues(bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, bool wateredToday, DateStruct nextWatering){
    this->enabled=enabled;
    this->intervalDays=intervalDays;
    this->wtrTime=wtrTime;
    this->waterAmmount=waterAmmount;
    this->wateredToday=wateredToday;
    this->nextWatering=nextWatering;
    index = globalIndexCounter++;
}

bool Schedule::update(TimeStruct currentTime) {

    if( enabled && waterAmmount > 0 ){
        if (!wateredToday && currentTime.isLaterThan(wtrTime)) {  //TODO dodać daty
            lcd->clear();
            lcd->setCursor(0, 0);
            String message = String(" Irrigating [")+ String(pump->id) + String("]");
            lcd->print(message);
            lcd->setCursor(0, 1);
            lcd->print("  Please wait..");
    
            pump->setState(true);
            delay(waterAmmount); 
            pump->setState(false);
            wateredToday=true;

            return 1;
        }
    }
    return 0;
}

void Schedule::updateEEPROM(){
    EEPROM->writeSchedule(index, getEnabled() , getInterval(), getWtrTime(), getAmmount());

}

unsigned long Schedule::getAmmount(){
    return waterAmmount;
}

void Schedule::setAmmount(String ammount){
    waterAmmount= ammount.toInt();
    updateEEPROM();
}

unsigned long Schedule::getInterval() {
    return intervalDays;
}

void Schedule::setInterval(String newInterval) {
    intervalDays = newInterval.toInt();
    updateEEPROM();
}

bool Schedule::getEnabled(){
    return enabled;
}

void Schedule::setEnabled(bool input){
    enabled = input;
    wateredToday = false;
    updateEEPROM();
}

TimeStruct Schedule::getWtrTime(){
    return wtrTime;
}

void Schedule::setWtrTime(TimeStruct input){
    wtrTime = input;
    updateEEPROM();
}

void Schedule::setWtrTime(String input){
    wtrTime = TimeStruct( input.substring(0,2).toInt(), input.substring(2,4).toInt() );
    updateEEPROM();
}