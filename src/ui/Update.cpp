#include "Menu.h"


void Menu::update(char key) {
    if (key) {
        if(!isSubmenu && !isEditing){           //MAIN MENU
            if (key == '4') {  // Lewo
                currentScreen = (currentScreen == PUMP1) ? SETTINGS : (MenuScreen)(currentScreen - 1);
            } else if (key == '6') {  // Prawo
                currentScreen = (currentScreen == SETTINGS) ? PUMP1 : (MenuScreen)(currentScreen + 1);
            } else if (key == '*') {  
                isSubmenu = true;
            }else if (key == '#') {  
                currentScreen = PUMP1;
            }
        }else if(isSubmenu){
            if(currentScreen == SETTINGS){
                if (key == '4') {  // Lewo
                    currentMenuSettingsScreen = (currentMenuSettingsScreen == TIME) ? ABOUT : (MenuSettingsScreen)(currentMenuSettingsScreen - 1);
                } else if (key == '6') {  // Prawo
                    currentMenuSettingsScreen = (currentMenuSettingsScreen == ABOUT) ? TIME : (MenuSettingsScreen)(currentMenuSettingsScreen + 1);
                }else if (key == '*') {  
                    if (currentMenuSettingsScreen == ADMIN)
                    {
                        adminFlag=!adminFlag;
                        
                    }else if (currentMenuSettingsScreen != ABOUT)
                    {
                        isEditing = true;
                        isSubmenu = false;
                    }                  
                    

                }else if (key == '#') {  // Zamknięcie submenu
                    isSubmenu = false;
                    currentMenuSettingsScreen = TIME;
                }

            }else if(currentScreen == RELAY){
                if (key == '#') {  // Zamknięcie submenu
                    isSubmenu = false;
                } 
            }else{ //Pump 1-3
                
                if (key == '4') {  // Lewo
                    currentSubScreen = (currentSubScreen == ENABLE) ? NEXT : (MenuSubScreen)(currentSubScreen - 1);
                } else if (key == '6') {  // Prawo
                    currentSubScreen = (currentSubScreen == NEXT) ? ENABLE : (MenuSubScreen)(currentSubScreen + 1);
                }else if (key == '0') {  // Prawo
                    schedules[currentScreen]->setNextWatering(currentDate+schedules[currentScreen]->getInterval());
                }else if (key == '#') {  // Zamknięcie submenu
                    isSubmenu = false;
                    currentSubScreen = ENABLE;
                } else if (key == '*') {  // Otwarcie edycji
                    if (currentSubScreen != ENABLE) {
                        isEditing = true;
                        isSubmenu = false;

                    }else{
                        schedules[currentScreen]->setEnabled(!(schedules[currentScreen]->getEnabled()));
                    }
                }
            }
        }else{  //editing
            if (key == '*') { 
                if(currentScreen==SETTINGS){
                    switch (currentMenuSettingsScreen)
                    {
                    case TIME:
                        while (inputBuffer.length()<4)
                        {
                            inputBuffer+="0";
                        }
                        setCurrentTime(TimeStruct(inputBuffer.substring(0,2).toInt(), inputBuffer.substring(2,4).toInt()), true);
                    break;

                    case DATE:
                        while (inputBuffer.length()<8)
                        {
                            inputBuffer+="0";
                        }

                        setCurrentDate(DateStruct(inputBuffer.substring(0,2).toInt(), inputBuffer.substring(2,4).toInt(), inputBuffer.substring(4,8).toInt()),true);

                    break;
                    case SLEEPING:
                        powerManager->setNoInteractionThreshhold(inputBuffer.toInt()*1000);
                        EEPROM->saveSettings(inputBuffer.toInt()*1000);
                    break;
                    
                    default:
                        break;
                    }
                    
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
            }else if(key >= '0' && key <= '9' && inputBuffer.length() < 8){
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

    case NEXT:
        while (inputBuffer.length()<8)
        {
            inputBuffer+="0";
        }
        schedules[currentScreen]->setNextWatering(DateStruct(inputBuffer.substring(0,2).toInt(), inputBuffer.substring(2,4).toInt(), inputBuffer.substring(4,8).toInt()));
    break;

    default:
        break;
    }
}