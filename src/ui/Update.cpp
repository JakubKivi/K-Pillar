#include "Menu.h"


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
            if(currentScreen == SETTINGS){
                if (key == '*') {  
                    isEditing = true;
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
                if(currentScreen==SETTINGS){
                    while (inputBuffer.length()<4)
                    {
                        inputBuffer+="0";
                    }
                    setCurrentTime(TimeStruct(inputBuffer.substring(0,2).toInt(), inputBuffer.substring(2,4).toInt()), true);
                }else{
                    updateSchedule();
                }
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

void Menu::updateSchedule(){
    switch (currentSubScreen)
    {
    case FREQ:
        schedules[currentScreen]->setInterval(inputBuffer);
        break;
    case TIMING:
        while (inputBuffer.length()<4)
        {
            inputBuffer+="0";
        }
        
        schedules[currentScreen]->setWtrTime(inputBuffer);
        break;
    case AMMOUNT:
        schedules[currentScreen]->setAmmount(inputBuffer);
        break;
    default:
        break;
    }
}