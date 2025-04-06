#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "logic/Schedule.h"

String formatTime(unsigned long milliseconds);
String formatAmount(unsigned long time);
String centerText(const char* text);

void lcdCreateCustomCharacters(LiquidCrystal_I2C *lcd);
void lcdCreateHomeScreen(LiquidCrystal_I2C *lcd);

void lcdDrawMenu(LiquidCrystal_I2C *lcd, int currentScreen, unsigned long interval);
void lcdDrawSubMenu(LiquidCrystal_I2C *lcd, int currentSubScreen, Schedule *schedule);
void lcdDrawEditing(LiquidCrystal_I2C *lcd);

#endif  // UTILS_H