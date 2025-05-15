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

Pump pump1(A0, 1);
Pump pump2(A1, 2);
Pump pump3(A2, 3);
Relay relay(A3);

Schedule schedule1(&pump1, 0, 1, TimeStruct(17, 0), 3000, &EEPROM, &lcd);
Schedule schedule2(&pump2, 0, 8, TimeStruct(17, 0), 2000, &EEPROM, &lcd);
Schedule schedule3(&pump3, 0, 9, TimeStruct(17, 0), 7000, &EEPROM, &lcd);

Schedule* schedules[] = {&schedule1, &schedule2, &schedule3};
Menu menu(&lcd, &keypad, schedules, &RTC);  

PowerManager powerManager(&lcd, &keypad, 8000);



void setup() {
    // for (int i = 0; i < 4; i++)
    // {
    //     EEPROM.writeSchedule(i, schedules[i]->getEnabled() , schedules[i]->getInterval(), schedules[i]->getWtrTime(), schedules[i]->getAmmount());
    // }
    bool enabled[NUM_SCHEDULES];
    unsigned int intervalDays[NUM_SCHEDULES];
    TimeStruct times[NUM_SCHEDULES];
    unsigned long waterAmmount[NUM_SCHEDULES];

    EEPROM.readAllSchedules(enabled, intervalDays, times, waterAmmount);
    for (int i = 0; i < 2; i++){
        schedules[i]->setValues(enabled[i],intervalDays[i],times[i],waterAmmount[i]);
    }

    
    Serial.begin(9600);
    lcd.init();
    powerManager.wakeUp();
    menu.lcdCreateCustomCharacters();
    menu.lcdCreateHomeScreen();
    menu.displayScreen();

    menu.setCurrentTime(TimeStruct(RTC.getHours(),RTC.getMinutes()), false);

	//RTC.setDate(13,05,25);
	// RTC.setTime(17,27,20);
    if(RTC.isConnected() == false)
	{
		Serial.println("RTC Not Connected!");
    }
}



void loop() {
    // delay(INT32_MAX);
    powerManager.update();
    char key = keypad.getKey();
    if (key) {
        // Serial.println(key);
        powerManager.resetTimer();  // Reset licznika usypiania
        
        menu.update(key);
    }
    //Serial.println(RTC.getDateTimeString());


    //menu.setCurrentTime(TimeStruct(RTC.getHours(),RTC.getMinutes()), false);
    
    if(schedule1.update(menu.getCurrentTime()) or schedule2.update(menu.getCurrentTime()) or schedule3.update(menu.getCurrentTime())) 
        menu.displayScreen();   

    delay(100);
    
}
