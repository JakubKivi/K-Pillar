#include "menu.h"


void Menu::lcdDrawSubMenu(){
    lcd->clear();
    lcd->setCursor(0, 0);
    if(currentScreen == SETTINGS){
            lcd->write(byte(0));
            lcd->print("4    Time    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(getCurrentTime()).c_str()));
    }else{   // PUMP1, PUMP2, PUMP3, RELAY
        String line;
        Serial.println(currentSubScreen);
        switch (currentSubScreen)
        {
        case ENABLE: 
            lcd->write(byte(0));
            lcd->print("4  * Toggle  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            if (schedules[currentScreen]->getEnabled())
            {
                lcd->print(centerText("Enabled"));
            }else{
                lcd->print(centerText("Disabled"));
            }
            
        break;

        case FREQ: 
            lcd->write(byte(0));
            lcd->print("4    Every   6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            line = String(schedules[currentScreen]->getInterval());
            line += " days";
            lcd->print(centerText( line.c_str() ));
        break;

        case TIMING: //ammount
            lcd->write(byte(0));
            lcd->print("4    Time    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
        
            lcd->print(centerText( formatTime(schedules[currentScreen]->getWtrTime()).c_str()));
        break;

        case AMMOUNT: //ammount
            lcd->write(byte(0));
            lcd->print("4  Duration  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
        
            lcd->print(centerText( formatAmount(schedules[currentScreen]->getAmmount()).c_str()));
        break;

        default:
            break;
        }
    }
}