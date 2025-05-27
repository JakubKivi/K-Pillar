#include "Schedule.h"
#include "hardware/EepromControl.h"

int Schedule::globalIndexCounter = 0;


Schedule::Schedule(Pump* pump, bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, EepromControl* EEPROM,  LiquidCrystal_I2C* lcd)
    : pump(pump), enabled(enabled), intervalDays(intervalDays), wtrTime(wtrTime), waterAmmount(waterAmmount), EEPROM(EEPROM), lcd(lcd) {}

void Schedule::setValues(bool enabled, unsigned int intervalDays, TimeStruct wtrTime, unsigned long waterAmmount, DateStruct nextWatering){
    this->enabled=enabled;
    this->intervalDays=intervalDays;
    this->wtrTime=wtrTime;
    this->waterAmmount=waterAmmount;
    this->nextWatering=nextWatering;
    index = globalIndexCounter++;
}

bool Schedule::update(TimeStruct currentTime, DateStruct currentDate) {

    if( enabled && waterAmmount > 0 ){
        int diff = nextWatering.diffDays(currentDate);
        if ( diff < 0 || (diff == 0 && currentTime.isLaterThan(wtrTime)))
        {
            lcd->clear();
            lcd->setCursor(0, 0);
            String message = String(" Irrigating [")+ String(pump->id) + String("]");
            lcd->print(message);
            lcd->setCursor(0, 1);
            lcd->print("  Please wait..");
    
            pump->setState(true);
            delay(waterAmmount); 
            pump->setState(false);
            
            setNextWatering(nextWatering += intervalDays);
            
            return 1;
        }
    }
    return 0;
}

void Schedule::updateEEPROM(){
    EEPROM->writeSchedule(index, enabled, intervalDays, wtrTime, waterAmmount, nextWatering);

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


DateStruct Schedule::getNextWatering(){
    return nextWatering;
}

void Schedule::setNextWatering(DateStruct nextWatering){
    this->nextWatering = nextWatering; 
    updateEEPROM();
}

bool Schedule::getEnabled(){
    return enabled;
}

void Schedule::setEnabled(bool input){
    enabled = input;
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