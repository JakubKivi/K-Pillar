#include "Menu.h"

String Menu::formatEditTime(String input){
    while(input.length()<4){
        input+="0";
    }
    return input.substring(0,2)+":"+input.substring(2,4);
}

void Menu::lcdDrawEditing(){
    lcd->clear();
    lcd->setCursor(0, 0);
    switch (currentSubScreen)
    {
    case ENABLE:
        lcd->print(centerText("You shouldn't"));
        lcd->setCursor(0, 1);
        lcd->print(centerText("be here o.o"));
    break;

    case FREQ: //freq
        lcd->print(centerText("Every"));
        lcd->setCursor(0, 1);
        lcd->print(centerText(  ( inputBuffer+" days").c_str()     )    );
    break;

    case TIMING: //ammount
        lcd->print(centerText("Time"));
        lcd->setCursor(0, 1);
        lcd->print(centerText(  formatEditTime(inputBuffer).c_str()  )  );
    break;

    case AMMOUNT: //ammount
        lcd->print(centerText("Duration"));
        lcd->setCursor(0, 1);
        lcd->print(centerText(  String(fillEmpty(inputBuffer) + "ms").c_str()  )  );
    break;

    
    default:
        break;
    }
}
