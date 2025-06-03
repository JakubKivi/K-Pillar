#include "menu.h"

void Menu::lcdDrawSubMenu()
{
    lcd->clear();
    lcd->setCursor(0, 0);
    if (currentScreen == SETTINGS)
    {
        String text;
        switch (currentMenuSettingsScreen)
        {
        case TIME:
            lcd->write(byte(0));
            lcd->print("4    Time    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(getCurrentTime()).c_str()));
            break;

        case DATE:
            lcd->write(byte(0));
            lcd->print("4    Date    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            text = String(currentDate.day) + "." + String(currentDate.month) + "." + String(currentDate.year) + "r";
            lcd->print(centerText(text.c_str()));
            break;

        case SLEEPING:
            lcd->write(byte(0));
            lcd->print("4 Sleep Time 6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            text = String(powerManager->getNoInteractionThreshhold());
            lcd->print(centerText(String(text.substring(0, text.length() - 3) + " [s]").c_str()));
            break;

        case ADMIN:
            lcd->write(byte(0));
            lcd->print("4    Admin   6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            text = String(adminFlag);
            lcd->print(centerText(text.c_str()));
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
    }
    else if (currentScreen == RELAY)
    {
        switch (currentRelaySubScreen)
        {
        case RENABLE:
            lcd->write(byte(0));
            lcd->print("4  * Toggle  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            if (schedules[currentScreen]->getEnabled())
            {
                lcd->print(centerText("Enabled"));
            }
            else
            {
                lcd->print(centerText("Disabled"));
            }
            break;
            
        case RTIME:
            lcd->write(byte(0));
            lcd->print("4   Time On  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(schedules[currentScreen]->getWtrTime()).c_str()));
            break;

        case RTIMEOFF:
            lcd->write(byte(0));
            lcd->print("4  Time Off  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(static_cast<RelaySchedule *>(schedules[currentScreen])->getTimeOff()).c_str()));
            break;

        default:
            break;
        }
    }
    else
    { // PUMP 1 - 3
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
            }
            else
            {
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
            lcd->print(centerText(line.c_str()));
            break;

        case TIMING: // ammount
            lcd->write(byte(0));
            lcd->print("4    Time    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);

            lcd->print(centerText(formatTime(schedules[currentScreen]->getWtrTime()).c_str()));
            break;

        case AMMOUNT: // ammount
            lcd->write(byte(0));
            lcd->print("4  Duration  6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);

            lcd->print(centerText(formatAmount(schedules[currentScreen]->getAmmount()).c_str()));
            break;

        case NEXT: // next
            lcd->write(byte(0));
            lcd->print("4    Next    6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            DateStruct date = schedules[currentScreen]->getNextWatering();
            line = String(date.day) + "." + String(date.month) + "." + String(date.year) + "r.";
            lcd->print(centerText(line.c_str()));
            break;

        default:
            break;
        }
    }
}