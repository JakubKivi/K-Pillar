#include "utils.h"
#include <Arduino.h>

String formatTime(unsigned long milliseconds) {
    unsigned long totalMinutes = milliseconds / 60000;  // Konwersja na minuty
    unsigned long minutes = totalMinutes % 60;
    unsigned long totalHours = totalMinutes / 60;
    unsigned long hours = totalHours % 24;
    unsigned long days = totalHours / 24;

    return String(days) + "d " + String(hours) + "h " + String(minutes) + "m ";
}