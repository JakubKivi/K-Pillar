#include "Menu.h"
#include "Arduino.h"

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

Menu::Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[], DS1307* RTC)
    : lcd(lcd), keypad(keypad), RTC(RTC) {
    for (int i = 0; i < 3; i++) {
        this->schedules[i] = schedules[i];
    }
    //currentTime = TimeStruct( RTC->getHours(), RTC->getMinutes() );
}
  
String Menu::formatTime(TimeStruct time) {

    String output="";
    if (time.hour<10)
    {
        output+="0";
    }
    output+=time.hour;
    output+=":";
    if (time.minute<10)
    {
        output+="0";
    }
    output+=time.minute;

    return output;

    // unsigned long totalMinutes = milliseconds / 60000;  // Konwersja na minuty
    // unsigned long minutes = totalMinutes % 60;
    // unsigned long totalHours = totalMinutes / 60;
    // unsigned long hours = totalHours % 24;
    // unsigned long days = totalHours / 24;

    // return String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
}
  

String Menu::formatAmount(unsigned long ammount) {
    return String(ammount) + " ms";
  }

String Menu::centerText(const char* text) {
    int len = strlen(text);
    if (len >= 16) return String(text).substring(0, 16);
  
    int spaces = (16 - len) / 2;
    String result = "";
    for (int i = 0; i < spaces; i++) result += ' ';
    result += text;
    return result;
  }



void Menu::displayScreen() {
    lcd->clear();
    if (isSubmenu){
        lcdDrawSubMenu();
        
    }else if (isEditing){
        lcdDrawEditing();

    }else{
        lcdDrawMenu(); 

    }
}

MenuScreen Menu::getCurrentScreen() const{ 
    return currentScreen;
}

void Menu::lcdDrawMenu(){
    lcd->clear();
        switch (currentScreen) {
            case PUMP1:
            case PUMP2:
            case PUMP3:  
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4   * Edit   6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print("     PUMP ");
                lcd->print(currentScreen + 1);
                break;
            case RELAY: 
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4   * Edit   6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print("     RELAY");
                break;
            case SETTINGS:  
                lcd->setCursor(0, 0);
                lcd->write(byte(0));
                lcd->print("4  * Select  6");
                lcd->write(byte(1));
                lcd->setCursor(0, 1);
                lcd->print(centerText("Settings"));
                break;
        }
}

String Menu::fillEmpty(String input){
    while(input.length()<6)
        input+=" ";
    return input;
}

void Menu::lcdCreateCustomCharacters(){
    lcd->createChar(0, leftArrow);
    lcd->createChar(1, rightArrow);
}

void Menu::lcdCreateHomeScreen(){
    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print(centerText("K-Pillar"));
    lcd->setCursor(0,1);
    lcd->print(centerText("by JakubKivi"));
    delay(1000);
}

TimeStruct Menu::getCurrentTime(){
    return currentTime;
}

void Menu::setCurrentTime(TimeStruct input, bool updateRTC){
    if(updateRTC)
        RTC->setTime(input.hour,input.minute,0);
        RTC->startClock();
    currentTime = input;
}