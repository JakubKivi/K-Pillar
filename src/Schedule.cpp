#include "Schedule.h"

Schedule::Schedule(Pump* pump, unsigned long interval) : pump(pump), interval(interval), lastWatered(0) {}

void Schedule::update() {
    if (millis() - lastWatered >= interval) {
        pump->setState(true);
        delay(5000); // Podlewanie przez 5 sekund
        pump->setState(false);
        lastWatered = millis();
    }
}

void Schedule::setInterval(unsigned long newInterval) {
    interval = newInterval;
}

unsigned long Schedule::getInterval() {
    return interval;
}
