/* 
 * Project: IoT Duck Coop
 * Author: Squiggy Rubio
 * Description: 
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "dawnDuskTimes.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int PHOTODIODE_DAYLIGHT = A3;
const int MOTIONSENSOR = D7;

const int HALLEFFECT_DOOR1  = D16;
const int HALLEFFECT_DOOR2  = D9;

const int DIR_DOOR1 = D3;
const int STEP_DOOR1 = D4;

const int DIR_DOOR2 = D5;
const int STEP_DOOR2 = D6;

const int IFRED_DOOR1 = A0;
const int IFRED_DOOR2 = A1;

const int PHOTODIODE_DOOR1 = A2;
const int PHOTODIODE_DOOR2 = A5;

const int SWITCH_STOP_OPEN_DOOR1 = D8;
const int SWITCH_STOP_CLOSE_DOOR1 = D15;

// TODO - wire swiches for Door 2
// const int SWITCH_STOP_OPEN_DOOR2 = 0;
// const int SWITCH_STOP_CLOSE_DOOR2 = 1;


const int microMode = 1; // microstep mode, default is 1/16 so 16; ex: 1/4 would be 4
// full rotation * microstep divider
const int steps = 200 * microMode;

// Daylight
int photodiodeDaylight;
int dusk;
int dawn;

int motionSensor;

int photodiodeDoor1;
int photodiodeDoor2;


void setup() {

  // Enable Serial Monitor
  Serial.begin(9600);
  waitFor( Serial.isConnected,10000); // wait for Serial monitor

  pinMode(PHOTODIODE_DAYLIGHT, INPUT);

  pinMode(MOTIONSENSOR, INPUT);

  pinMode(HALLEFFECT_DOOR1, INPUT);
  pinMode(HALLEFFECT_DOOR2, INPUT);

  // setup step and dir pins as outputs
  pinMode(DIR_DOOR1, OUTPUT);
  pinMode(STEP_DOOR1, OUTPUT);

  pinMode(DIR_DOOR2, OUTPUT);
  pinMode(STEP_DOOR2, OUTPUT);

  pinMode(IFRED_DOOR1, OUTPUT);
  pinMode(IFRED_DOOR2, OUTPUT);

  pinMode(PHOTODIODE_DOOR1, INPUT);
  pinMode(PHOTODIODE_DOOR2, INPUT);

  // Time strings 

  Time.zone(-7);
  Particle.syncTime();



}

void loop() {

 ////// ------------------------- Test Motors ------------------------- //////

  // // change direction every loop
  // digitalWrite(DIR_DOOR1, !digitalRead(DIR_DOOR1));
  // // toggle STEP to move
  // for(int x = 0; x < steps; x++)
  // {
  //   digitalWrite(STEP_DOOR1, HIGH);
  //   delay(2);
  //   digitalWrite(STEP_DOOR1, LOW);
  //   delay(2);
  // }
  // delay(1000); // 1 second delay

  //   // change direction every loop
  // digitalWrite(DIR_DOOR2, !digitalRead(DIR_DOOR2));
  // // toggle STEP to move
  // for(int x = 0; x < steps; x++)
  // {
  //   digitalWrite(STEP_DOOR2, HIGH);
  //   delay(2);
  //   digitalWrite(STEP_DOOR2, LOW);
  //   delay(2);
  // }
  // delay(1000); // 1 second delay

  analogWrite(IFRED_DOOR1, 128);  // Laser ON
  analogWrite(IFRED_DOOR2, 128);  // Laser ON

  motionSensor = digitalRead(MOTIONSENSOR);
  // Serial.printf("motionSensor %i \n", motionSensor);

////// ------------------------- Daylight ------------------------- //////

  // Check photodiode for daylight
  photodiodeDaylight = analogRead(PHOTODIODE_DAYLIGHT);
  Serial.printf("photodiodeDaylight %i \n", photodiodeDaylight);
  
  if (photodiodeDaylight) {
    // Unix timestamp
    time32_t now();
    // Print the current Unix timestamp
    Serial.printf("time is: %d \n", (int) Time.now());
    // Serial.print((int) Time.now()); // 1400647897

    // Time string
    // dateTime = Time.timeStr();
    // timestamp = dateTime.substring(11,19);
    // Serial.printf("%s \n", dateTime.c_str());

    // TODO - save daylight reading and time to Red-Node dashboard. Determine way to keep 
    // track of time for dawn and dusk, as this time changes.
    // dusk = ;
  }
 
////// ------------------------- Outer door ------------------------- //////

  photodiodeDoor1 = analogRead(PHOTODIODE_DOOR1);
  if (photodiodeDoor1 > 50) {
    // Open door so duck doesn't get squished!
    // digitalWrite(STEP_DOOR1, HIGH);
  }
  // Serial.printf("photodiodeDoor1 %i \n", photodiodeDoor1);

////// ------------------------- Inner door ------------------------- //////

  photodiodeDoor2 = analogRead(PHOTODIODE_DOOR2);
  if (photodiodeDoor1 > 50) {
    // Open door so duck doesn't get squished!
        digitalWrite(STEP_DOOR2, LOW);
  }
  // Serial.printf(" %i \n", photodiodeDoor2);
}
