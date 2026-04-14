/* 
 * Project: IoT Duck Coop
 * Author: Squiggy Rubio
 * Description: 
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "dawnDuskTimes.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

const int MOTIONSENSOR = D7;

////// ------------------------- Open door / Close door ------------------------- //////

bool switchMotorOff;

////// ------------------------- Door 1 / Outer Door ------------------------- //////

const int HALLEFFECT_DOOR1  = D16;
int hallEffectReadingDoor1; 

const int DIR_DOOR1 = D3;
const int STEP_DOOR1 = D4;

const int LASERDIODE_DOOR1 = A0;

const int PHOTODIODE_DOOR1 = A2;

const int SWITCH_STOP_OPENING_DOOR1 = D8;
const int SWITCH_STOP_CLOSEING_DOOR1 = D15;

bool isOuterDoorOpen();
void openOuterDoor();
void closeOuterDoor();

////// ------------------------- Door 2 / Inner Door ------------------------- //////

const int HALLEFFECT_DOOR2  = D9;

const int DIR_DOOR2 = D5;
const int STEP_DOOR2 = D6;

const int LASERDIODE_DOOR2 = A1;

const int PHOTODIODE_DOOR2 = A5;

// TODO - wire swiches for Door 2
const int SWITCH_STOP_OPENING_DOOR2 = 0;
const int SWITCH_STOP_CLOSEING_DOOR2 = 0;

bool isInnerDoorOpen();
void openInnerDoor();
void closeInnerDoor();

// Set motor mode
const int microMode = 1; // microstep mode, default is 1/16 so 16; ex: 1/4 would be 4
// full rotation * microstep divider
const int steps = 200 * microMode;

 ////// ------------------------- Timers ------------------------- //////

int checkDawnTimer; // 5 minutes

////// ------------------------- Variables ------------------------- //////

int motionSensor;
int photodiodeDoor1;
int photodiodeDoor2;

void setup() {

  // Enable Serial Monitor
  Serial.begin(9600);
  waitFor( Serial.isConnected,10000); // wait for Serial monitor

  pinMode(MOTIONSENSOR, INPUT);

  pinMode(HALLEFFECT_DOOR1, INPUT);
  pinMode(HALLEFFECT_DOOR2, INPUT);

  // setup step and dir pins as outputs
  pinMode(DIR_DOOR1, OUTPUT);
  pinMode(STEP_DOOR1, OUTPUT);

  pinMode(DIR_DOOR2, OUTPUT);
  pinMode(STEP_DOOR2, OUTPUT);

  pinMode(LASERDIODE_DOOR1, OUTPUT);
  pinMode(LASERDIODE_DOOR2, OUTPUT);

  pinMode(PHOTODIODE_DOOR1, INPUT);
  pinMode(PHOTODIODE_DOOR2, INPUT);

  pinMode(SWITCH_STOP_CLOSEING_DOOR1, INPUT);
  pinMode(SWITCH_STOP_CLOSEING_DOOR2, INPUT);
  pinMode(SWITCH_STOP_OPENING_DOOR1, INPUT);
  pinMode(SWITCH_STOP_OPENING_DOOR2, INPUT);
  // WiFi.setCredentials("DDCIOT","ddcIOT2020");
  // WiFi.setCredentials("S Rubio's iPhone","bobmarley");

  // // Connect to Internet but not Particle Cloud
  WiFi.on();
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
    delay(100);
  }
  Serial.printf("\n\n");
  delay(3000);

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

  analogWrite(LASERDIODE_DOOR1, 128);  // Laser ON
  analogWrite(LASERDIODE_DOOR2, 128);  // Laser ON

////// ------------------------- Motion Sensor ------------------------- //////

  motionSensor = digitalRead(MOTIONSENSOR);
  // Serial.printf("motionSensor %i \n", motionSensor);
  // If there is motion in chamber, close outer door and open inner door to 
  // let in duck.
  if (motionSensor > 30) {
    // Close outer door
    closeOuterDoor();
    // Open inner door, to let ducks inside.
    openInnerDoor();
  }

  closeOuterDoor();

////// ------------------------- Daylight ------------------------- //////

  // Check photodiode for daylight
  // photodiodeDaylight = analogRead(PHOTODIODE_DAYLIGHT);
  // Serial.printf("photodiodeDaylight %i \n", photodiodeDaylight);

      if((millis() - checkDawnTimer) > 500) {
        if (isDawn() && isOuterDoorOpen() == false && isInnerDoorOpen() == false) {
          Serial.printf("checkDawnTimer %i", checkDawnTimer);
          openInnerDoor();
          openOuterDoor();
          // TODO - determine way to keep track of time for dawn and dusk, as this time changes through the year.
        }
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
  if (photodiodeDoor2 > 50) {
    // Open door so duck doesn't get squished!
        digitalWrite(STEP_DOOR2, LOW);
  }
  // Serial.printf(" %i \n", photodiodeDoor2);
}

void openOuterDoor() {

  int switchMotorOff = digitalRead(SWITCH_STOP_OPENING_DOOR1);
  Serial.printf("switchMotorOff %d \n", switchMotorOff);
  if (!switchMotorOff) {
    digitalWrite(STEP_DOOR1, HIGH);
  }

};

void closeOuterDoor() {

  int switchMotorOff = digitalRead(SWITCH_STOP_CLOSEING_DOOR1);

  if (!switchMotorOff) {
    digitalWrite(STEP_DOOR2, LOW);
  }

};

void openInnerDoor() {

};
void closeInnerDoor() {

};

bool isOuterDoorOpen() {
  ////// ------------------------- Door 1 / Outer Door ------------------------- //////
  // HALLEFFECT_DOOR1
  // hallEffectReadingDoor1 = digitalRead(HALLEFFECT_DOOR1);
  // if (hallEffectReadingDoor1) {
    
  // }
  // Test return value true for now. TODO - use halleffect sensor to check if door is open.
  return true;
};

bool isInnerDoorOpen() {
  ////// ------------------------- Door 2 / Inner Door ------------------------- //////
  // HALLEFFECT_DOOR2

    // Test return value true for now. TODO - use Halleffect sensor to check if door is open.
    return true;
};
