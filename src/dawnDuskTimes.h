#include "Particle.h"

bool isDusk();

bool isDawn();

////// ------------------------- Timers five days apart ------------------------- //////

int todayTimer;
int fiveDaysTimer;
int tenDaysTimer;
int fifteenDaysTimer;
int twentyDaysTimer;
int thirtyDaysTimer;

time32_t now();

const int PHOTODIODE_DAYLIGHT = A3;

int currentTime;
int dawn;
int dusk;
int photodiodLastReading;
int photodiodeDaylight;

bool isDusk() {
    return false;
}

bool isDawn() {

    // Print the current Unix timestamp
    currentTime = (int) Time.now();
    // Serial.printf("time is currentTime: %d \n", currentTime);

    // Check photodiode for daylight
    photodiodeDaylight = analogRead(PHOTODIODE_DAYLIGHT);
    // Serial.printf("photodiodeDaylight %i \n", photodiodeDaylight);

    if (photodiodeDaylight > photodiodLastReading) {


    }

    dusk = 1776168959; // current time April 14 ~3:15pm, minus 9 hours to get dawn 

    return true;
}

    // Unix timestamp
    // time32_t now();

    // // Print the current Unix timestamp
    // Serial.printf("time is: %d \n", (int) Time.now());
    // Serial.print((int) Time.now()); // 1400647897
    // getTimeDawn();

    // Time string
    // dateTime = Time.timeStr();
    // timestamp = dateTime.substring(11,19);
    // Serial.printf("%s \n", dateTime.c_str());
