#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

String formatTime(unsigned long milliseconds);
void lcdDrawMenu(LiquidCrystal_I2C *lcd, int currentScreen, unsigned long interval);

#endif  // UTILS_H