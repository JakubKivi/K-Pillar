#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "hardware/Pump.h"
#include "hardware/Relay.h"
#include "logic/Schedule.h"
#include "ui/Menu.h"
#include "power/PowerManager.h"
#include <Wire.h>
#include <I2C_RTC.h>
#include "hardware/EepromControl.h"
#include "logic/DateStruct.h"

#define NUM_SCHEDULES 3

static DS1307 RTC;

EepromControl EEPROM;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Pump pump1(11, 1);
Pump pump2(10, 2);
Pump pump3(9, 3);
Relay relay(A3);

Schedule schedule1(&pump1, 0, 1, TimeStruct(17, 0), 3000, &EEPROM, &lcd);
Schedule schedule2(&pump2, 0, 8, TimeStruct(17, 0), 2000, &EEPROM, &lcd);
Schedule schedule3(&pump3, 0, 9, TimeStruct(17, 0), 7000, &EEPROM, &lcd);

Schedule* schedules[] = {&schedule1, &schedule2, &schedule3};
PowerManager powerManager(&lcd, &keypad, 15000);
Menu menu(&lcd, &keypad, schedules, &RTC, &powerManager);  


void setup() {

    bool enabled[NUM_SCHEDULES];
    unsigned int intervalDays[NUM_SCHEDULES];
    TimeStruct times[NUM_SCHEDULES];
    unsigned long waterAmmount[NUM_SCHEDULES];
    DateStruct nextWatering[NUM_SCHEDULES];

    EEPROM.readAllSchedules(enabled, intervalDays, times, waterAmmount, nextWatering);
    for (int i = 0; i < 2; i++){
        schedules[i]->setValues(enabled[i],intervalDays[i],times[i],waterAmmount[i], nextWatering[i]);
    }                                                                                           //LOADING DATA FROM EEPROM

    Serial.begin(9600);
    lcd.init();
    powerManager.wakeUp();
    menu.lcdCreateCustomCharacters();
    menu.lcdCreateHomeScreen();
    menu.displayScreen();

    RTC.begin();
    if(RTC.isConnected() == false)
	{
		Serial.println("RTC Not Connected!");
    }else{
        if (!RTC.isRunning())
        {
            RTC.startClock();
        }
        
    }
    tm read = RTC.getDateTime();
    menu.setCurrentTime(TimeStruct(read.tm_hour ,read.tm_min), false);
    menu.setCurrentDate(DateStruct(read.tm_mday, read.tm_mon, read.tm_year),false);
    // RTC.setWeek(3);
	// RTC.setDate(20,05,25);
	// RTC.setTime(19,29,0);
}

unsigned int counter =0;

void loop() {
    powerManager.update();
    char key = keypad.getKey();

    if (key)
    {
        powerManager.resetTimer(); 

        if (powerManager.isSilentWakeUp){
            powerManager.isSilentWakeUp=false;
            lcd.backlight();
        }else{
            menu.update(key);
        }
    }
        
    counter++;
    if (counter>100)  //~10 sec
    {
        counter=0;
        Serial.println(RTC.getDateTimeString());
        tm read = RTC.getDateTime();
        menu.setCurrentTime(TimeStruct(read.tm_hour ,read.tm_min), false);
        menu.setCurrentDate(DateStruct(read.tm_mday, read.tm_mon, read.tm_year),false);
    }
        
    TimeStruct time = menu.getCurrentTime();
    DateStruct date = menu.getCurrentDate();
    if(schedule1.update(time, date) or schedule2.update(time, date) or schedule3.update(time, date)) 
        menu.displayScreen();   

    delay(100);
}
