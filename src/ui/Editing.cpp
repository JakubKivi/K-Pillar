#include "Menu.h"

String Menu::formatEditTime(String input){
    while(input.length()<4){
        input+="0";
    }
    return input.substring(0,2)+":"+input.substring(2,4);
}


String Menu::formatEditDate(String input){
    while(input.length()<8){
        input+="0";
    }
    return input.substring(0,2) + "." + input.substring(2,4) + "." + input.substring(4,8) + "r.";
}

void Menu::lcdDrawEditing(){
    lcd->clear();
    lcd->setCursor(0, 0);
    if (currentScreen == SETTINGS){
        switch (currentMenuSettingsScreen)
        {
        case TIME:
            lcd->setCursor(0, 0);
            lcd->print(centerText("Time"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditTime(inputBuffer).c_str()  )  );
        break;
        
        case DATE:
            lcd->setCursor(0, 0);
            lcd->print(centerText("Date"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditDate(inputBuffer).c_str()  )  );
        break;
            
        case SLEEPING:
            lcd->setCursor(0, 0);
            lcd->print(centerText("Sleep Time"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  String(inputBuffer +" [s]").c_str() )  );
        break;

        default:
            break;
        }

    }else if(currentScreen == RELAY){
        switch (currentRelaySubScreen)
        {
        case RTIME:
            lcd->setCursor(0, 0);
            lcd->print(centerText("Time On"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditTime(inputBuffer).c_str()  )  );
        break;

        case RTIMEOFF:
            lcd->setCursor(0, 0);
            lcd->print(centerText("Time Off"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditTime(inputBuffer).c_str()  )  );
        break;

        default:
            break;
        }

    }else{
        switch (currentSubScreen)
        {
        case ENABLE:
            lcd->print(centerText("You shouldn't"));
            lcd->setCursor(0, 1);
            lcd->print(centerText("be here o.o"));
        break;

        case FREQ: 
            lcd->print(centerText("Every"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  ( inputBuffer+" days").c_str()     )    );
        break;

        case TIMING: 
            lcd->print(centerText("Time"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditTime(inputBuffer).c_str()  )  );
        break;

        case AMMOUNT: 
            lcd->print(centerText("Duration"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  String(fillEmpty(inputBuffer) + "ms").c_str()  )  );
        break;

         case NEXT: 
            lcd->print(centerText("Next"));
            lcd->setCursor(0, 1);
            lcd->print(centerText(  formatEditDate(inputBuffer).c_str()  )  );
        break;
        
        default:
            break;
        }
    }
    
        
}
