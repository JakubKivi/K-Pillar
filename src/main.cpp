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
#include "logic/RelaySchedule.h"

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
    {'*', '0', '#'}};
byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Pump pump1(9, 1);
Pump pump2(10, 2);
Pump pump3(11, 3);
Relay relay(A3);

Schedule schedule1(&pump1, &EEPROM, &lcd);
Schedule schedule2(&pump2, &EEPROM, &lcd);
Schedule schedule3(&pump3, &EEPROM, &lcd);

RelaySchedule relaySchedule(&relay, &EEPROM, &lcd);

Schedule *schedules[] = {&schedule1, &schedule2, &schedule3, &relaySchedule};
PowerManager powerManager(&lcd, &keypad, 15000);
Menu menu(&lcd, &keypad, schedules, &RTC, &powerManager, &EEPROM);

void setup()
{

    bool enabled[NUM_SCHEDULES];
    unsigned int intervalDays[NUM_SCHEDULES];
    TimeStruct times[NUM_SCHEDULES];
    unsigned long waterAmmount[NUM_SCHEDULES];
    DateStruct nextWatering[NUM_SCHEDULES];

    // EEPROM.writeSchedule(0, 0, 2, TimeStruct(15,0), 3000, DateStruct(26,5,2025));
    // EEPROM.writeSchedule(1, 0, 3, TimeStruct(16,0), 6000, DateStruct(27,5,2025));
    // EEPROM.writeSchedule(2, 0, 4, TimeStruct(18,0), 8000, DateStruct(28,5,2025));

    //EEPROM.writeRelaySchedule(0, TimeStruct(18, 0), TimeStruct(22, 0));

    EEPROM.readAllSchedules(enabled, intervalDays, times, waterAmmount, nextWatering);
    for (int i = 0; i <= 2; i++)
    {
        schedules[i]->setValues(enabled[i], intervalDays[i], times[i], waterAmmount[i], nextWatering[i]);
    }

    bool rEnabled;
    TimeStruct rTime;
    TimeStruct rTimeOff;

    EEPROM.readRelaySchedule(&rEnabled, &rTime, &rTimeOff);

    relaySchedule.setValues(rEnabled, rTime, rTimeOff);

    uint16_t noInteractionThreshhold;
    EEPROM.readSettings(noInteractionThreshhold);
    powerManager.setNoInteractionThreshhold(noInteractionThreshhold);

    Serial.begin(9600);
    lcd.init();
    powerManager.wakeUp();
    menu.lcdCreateCustomCharacters();
    menu.lcdCreateHomeScreen();
    menu.displayScreen();

    RTC.begin();
    if (RTC.isConnected() == false)
    {
        Serial.println("RTC Not Connected!");
    }
    else
    {
        if (!RTC.isRunning())
        {
            RTC.startClock();
        }
    }
    tm read = RTC.getDateTime();
    menu.setCurrentTime(TimeStruct(read.tm_hour, read.tm_min), false);
    menu.setCurrentDate(DateStruct(read.tm_mday, read.tm_mon, 1900 + read.tm_year), false);

    // RTC.setWeek(3);
    // RTC.setDate(20,05,25);
    // RTC.setTime(19,29,0);

    char key = keypad.getKey();
    if (key == '*')
    {
        menu.adminFlag = true;
    }
}

unsigned int counter = 0;

void loop()
{
    if (powerManager.update())
    {
        menu.displayScreen();
    }
    
    char key = keypad.getKey();

    if (key)
    {
        powerManager.resetTimer();

        if (powerManager.isSilentWakeUp)
        {
            powerManager.isSilentWakeUp = false;
            lcd.backlight();
        }
        else
        {
            menu.update(key);
        }
    }

    counter++;
    if (counter > 100) //~10 sec
    {
        counter = 0;
        Serial.println(RTC.getDateTimeString());
        DateStruct a = menu.getCurrentDate();
        Serial.println(String(a.day)+"."+a.month+"."+a.year);
        tm read = RTC.getDateTime();
        menu.setCurrentTime(TimeStruct(read.tm_hour, read.tm_min), false);
        menu.setCurrentDate(DateStruct(read.tm_mday, read.tm_mon + 1, 1900 + read.tm_year), false);
        
        TimeStruct time = menu.getCurrentTime();
        DateStruct date = menu.getCurrentDate();

        if (!menu.adminFlag)
        {
            if (schedule1.update(time, date) or schedule2.update(time, date) or schedule3.update(time, date))
                menu.displayScreen(powerManager.isSilentWakeUp);
            relaySchedule.update(time, date);
        }
    }


    delay(100);
}
