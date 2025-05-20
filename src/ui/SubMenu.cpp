#include "menu.h"


void Menu::lcdDrawSubMenu(){
    lcd->clear();
    lcd->setCursor(0, 0);
    if(currentScreen == SETTINGS){
        switch (currentMenuSettingsScreen)
        {
        case TIME:
            lcd->write(byte(0));
            lcd->print("4    Time    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(getCurrentTime()).c_str()));
            break;

        case SLEEPING:
            lcd->write(byte(0));
            lcd->print("4 Sleep Time 6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print("dupsko tak o");
            break;

        case ABOUT:
            lcd->setCursor(0, 0);
            lcd->write(byte(0));
            lcd->print("4 Created by 6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print("   JakubKivi");
            break;

        default:
            break;
        }
            
    }else if(currentScreen == RELAY){
        lcd->setCursor(0, 0);
        lcd->print(centerText("TODO"));
        lcd->setCursor(0, 1);
        lcd->print(centerText("# to exit"));

    }else{   // PUMP 1 - 3
        String line;
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