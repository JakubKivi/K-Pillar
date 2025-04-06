#include "utils.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "logic/Schedule.h"

// Custom character for ◀️
byte leftArrow[8] = {
    B00010,
    B00110,
    B01110,
    B11110,
    B01110,
    B00110,
    B00010,
    B00000
  };
  
  // Custom character for ▶️
  byte rightArrow[8] = {
    B01000,
    B01100,
    B01110,
    B01111,
    B01110,
    B01100,
    B01000,
    B00000
  };

String formatTime(unsigned long milliseconds) {
    unsigned long totalMinutes = milliseconds / 60000;  // Konwersja na minuty
    unsigned long minutes = totalMinutes % 60;
    unsigned long totalHours = totalMinutes / 60;
    unsigned long hours = totalHours % 24;
    unsigned long days = totalHours / 24;

    return String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
}

String formatAmount(unsigned long time) {
    return String(time) + " ms";
  }

String centerText(const char* text) {
    int len = strlen(text);
    if (len >= 16) return String(text).substring(0, 16);
  
    int spaces = (16 - len) / 2;
    String result = "";
    for (int i = 0; i < spaces; i++) result += ' ';
    result += text;
    return result;
  }

void lcdCreateCustomCharacters(LiquidCrystal_I2C *lcd){
    lcd->createChar(0, leftArrow);
    lcd->createChar(1, rightArrow);
}

void lcdCreateHomeScreen(LiquidCrystal_I2C *lcd){
    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print(centerText("K-Pillar"));
    lcd->setCursor(0,1);
    lcd->print(centerText("by JakubKivi"));
    delay(2000);
}

void lcdDrawMenu(LiquidCrystal_I2C *lcd, int currentScreen, unsigned long interval){
    lcd->clear();
        switch (currentScreen) {
            case 0:
            case 1:
            case 2:  //pumps
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4   * Edit   6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print("     PUMP ");
                lcd->print(currentScreen + 1);
                break;
            case 3:  //relay
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4   * Edit   6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print("     RELAY");
                break;
            case 4:   //manual
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4  * Select  6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print("  Manual Mode");
                break;
                
            case 5: //about
            lcd->setCursor(0, 0);
            lcd->write(byte(0));
            lcd->print("4 Created by 6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print("   JakubKivi");
            break;
        }
}

void lcdDrawSubMenu(LiquidCrystal_I2C *lcd, int currentSubScreen, Schedule *schedule){
    lcd->clear();
    lcd->setCursor(0, 0);
    switch (currentSubScreen)
    {
    case 0: // enable
        lcd->write(byte(0));
        lcd->print("4  * Toggle  6");
        lcd->write(byte(1));
        lcd->setCursor(0, 1);
        if (schedule->getEnabled())
        {
            lcd->print(centerText("Enabled"));
        }else{
            lcd->print(centerText("Disabled"));
        }
        
    break;

    case 1:
        lcd->write(byte(0));
        lcd->print("4    Every   6");
        lcd->write(byte(1));
        lcd->setCursor(0, 1);
        lcd->print(centerText(formatTime(schedule->getInterval()).c_str()));
    break;

    case 2:
        lcd->write(byte(0));
        lcd->print("4  Duration  6");
        lcd->write(byte(1));
        lcd->setCursor(0, 1);
    
        lcd->print(centerText( formatAmount(schedule->getAmmount()).c_str()));
    break;
    
    default:
        break;
    }
}


void lcdDrawEditing(LiquidCrystal_I2C *lcd){
    lcd->clear();
    lcd->print("Edycja");
}