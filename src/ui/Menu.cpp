#include "Menu.h"

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

  
String Menu::formatTime(unsigned long milliseconds) {
    unsigned long totalMinutes = milliseconds / 60000;  // Konwersja na minuty
    unsigned long minutes = totalMinutes % 60;
    unsigned long totalHours = totalMinutes / 60;
    unsigned long hours = totalHours % 24;
    unsigned long days = totalHours / 24;

    return String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
}

String Menu::formatAmount(unsigned long time) {
    return String(time) + " ms";
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

String Menu::formatEditText(String text){
    
    text = fillEmpty(text);
    
    return text.substring(0,2)+"d"+text.substring(2,4)+"h"+text.substring(4,6)+"m";
}

Menu::Menu(LiquidCrystal_I2C* lcd, Keypad* keypad, Schedule* schedules[])
    : lcd(lcd), keypad(keypad) {
    for (int i = 0; i < 3; i++) {
        this->schedules[i] = schedules[i];
    }
}

void Menu::update(char key) {
    if (key) {
        if(!isSubmenu && !isEditing){
            if (key == '4') {  // Lewo
                currentScreen = (currentScreen == PUMP1) ? ABOUT : (MenuScreen)(currentScreen - 1);
            } else if (key == '6') {  // Prawo
                currentScreen = (currentScreen == ABOUT) ? PUMP1 : (MenuScreen)(currentScreen + 1);
            } else if (key == '*') {  // Otwarcie submenu
                if (currentScreen != ABOUT) {
                    isSubmenu = true;
                }
            }
        }else if(isSubmenu){
            if(currentScreen == MANUAL){
                if (key == '*') {  //idk to potem
                    isSubmenu = false;

                }else if (key == '#') {  // Zamknięcie submenu
                    isSubmenu = false;
                }
            }else{
                
                if (key == '4') {  // Lewo
                    currentSubScreen = (currentSubScreen == ENABLE) ? AMMOUNT : (MenuSubScreen)(currentSubScreen - 1);
                } else if (key == '6') {  // Prawo
                    currentSubScreen = (currentSubScreen == AMMOUNT) ? ENABLE : (MenuSubScreen)(currentSubScreen + 1);
                }else if (key == '#') {  // Zamknięcie submenu
                    isSubmenu = false;
                } else if (key == '*') {  // Otwarcie edycji
                    if (currentSubScreen != ENABLE) {
                        isEditing = true;
                        isSubmenu = false;

                    }else{
                        schedules[currentScreen]->setEnabled(!(schedules[currentScreen]->getEnabled()));
                    }
                }
            }
        }else{
            if (key == '*') { 
                updateSchedule();
                isSubmenu = true;
                isEditing=false;
                inputBuffer="";
            }else if (key == '#') {  // Powrót do submenu
                if(inputBuffer == ""){
                    isSubmenu = true;
                    isEditing=false;
                }else{
                    inputBuffer="";
                }
            }else if(key >= '0' && key <= '9' && inputBuffer.length() < 6){
                inputBuffer += key;
            }
        }
        displayScreen();
    } 
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

void Menu::lcdDrawSubMenu(){
    lcd->clear();
    lcd->setCursor(0, 0);
    if(currentScreen == MANUAL){
        lcd->print("TODO");
    }else{
        switch (currentSubScreen)
        {
        case 0: // enable
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

        case 1: //freq
            lcd->write(byte(0));
            lcd->print("4    Every   6");
            lcd->write(byte(1));
            lcd->setCursor(0, 1);
            lcd->print(centerText(formatTime(schedules[currentScreen]->getInterval()).c_str()));
        break;

        case 2: //ammount
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

void Menu::lcdDrawEditing(){
    lcd->clear();
    lcd->setCursor(0, 0);
    switch (currentSubScreen)
    {
    case 1: //freq
        lcd->print(centerText("Every"));
        lcd->setCursor(0, 1);
        lcd->print(centerText(  formatEditText(inputBuffer).c_str()     )    );
    break;

    case 2: //ammount
        lcd->print(centerText("Duration"));
        lcd->setCursor(0, 1);
        lcd->print(centerText(  String(fillEmpty(inputBuffer) + "ms").c_str()  )  );
    break;

    
    default:
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
    delay(2000);
}

void Menu::updateSchedule(){
    if (currentSubScreen == FREQ)
    {
        // schedules[currentScreen]->setInterval(inputBuffer);
    }else{
        // schedules[currentScreen]->setAmmount(inputBuffer);
    }
}