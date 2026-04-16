/* 
 * Project: IoT Duck Coop
 * Author: Squiggy Rubio
 * Description: 
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "dawnDuskTimes.h"
#include "iotduckcoopTests.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

////// ------------------------- Shared variables ------------------------- //////

const int MOTIONSENSOR = D7;
int motionSensor;

// const int PHOTODIODE_DAYLIGHT = A3;
const int LED_DAYLIGHT_TEST = A4;
int brightness;

int t;

////// ------------------------- Door 1 / Outer Door ------------------------- //////

const int HALLEFFECT_DOOR1  = D16;
int hallEffectReadingDoor1; 

const int DIR_DOOR1 = D3;
const int STEP_DOOR1 = D4;

const int LASERDIODE_DOOR1 = A0;

const int PHOTODIODE_DOOR1 = A2;
int photodiodeDoor1;

const int SWITCH_STOP_OPENING_DOOR1 = D17;
const int SWITCH_STOP_CLOSEING_DOOR1 = D18;
bool switchStopOpeningDoor1;

bool isOuterDoorOpen();
void openOuterDoor();
void closeOuterDoor();

////// ------------------------- Door 2 / Inner Door ------------------------- //////

const int HALLEFFECT_DOOR2  = D9;

const int DIR_DOOR2 = D5;
const int STEP_DOOR2 = D6;

const int LASERDIODE_DOOR2 = A1;

const int PHOTODIODE_DOOR2 = A5;
int photodiodeDoor2;

// TODO - wire swiches for Door 2
const int SWITCH_STOP_OPENING_DOOR2 = D15;
const int SWITCH_STOP_CLOSEING_DOOR2 = D8;
bool switchStopOpeningDoor2;

bool isInnerDoorOpen();
void openInnerDoor();
void closeInnerDoor();

// Set motor mode
const int microMode = 1; // microstep mode, default is 1/16 so 16; ex: 1/4 would be 4
// full rotation * microstep divider
const int steps = 200 * microMode;

////// ------------------------- Timers ------------------------- //////

int checkDawnTimer; // 5 minutes


void setup() {

  // Enable Serial Monitor
  Serial.begin(9600);
  waitFor( Serial.isConnected,10000); // wait for Serial monitor

  pinMode(MOTIONSENSOR, INPUT);
  pinMode(LED_DAYLIGHT_TEST, OUTPUT);

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

  pinMode(SWITCH_STOP_CLOSEING_DOOR1, INPUT_PULLDOWN);
  pinMode(SWITCH_STOP_CLOSEING_DOOR2, INPUT_PULLDOWN);
  pinMode(SWITCH_STOP_OPENING_DOOR1, INPUT_PULLDOWN);
  pinMode(SWITCH_STOP_OPENING_DOOR2, INPUT_PULLDOWN);
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
 ////// ------------------------- Tests ------------------------- //////

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
  Serial.printf("motionSensor %i \n", motionSensor);
  // If there is motion in chamber, close outer door and open inner door to 
  // let in duck.

 // If it's dark, let ducks inside coop if motion is detected 
  if (motionSensor && photodiodeDaylight < 15) {
    // close outer door
    closeOuterDoor(); // DONE
    Serial.printf("Line 169: closeOuterDoor()! \n");
    // open inner door
    openInnerDoor();
    Serial.printf("Line 172: openInnerDoor()! \n");
  }

  if (motionSensor == 0) {
    openOuterDoor(); // DONE
    closeInnerDoor();
  }

////// ------------------------- Daylight ------------------------- //////

  // Use LED to simulate daylight (for testing)
  t = millis() / 5000.0;
  brightness = 127.5 * sin(2*M_PI*(1/5.0)*t)+127.5;
  analogWrite(LED_DAYLIGHT_TEST, brightness);

  // Use photodiode to detect daylight.
  photodiodeDaylight = analogRead(PHOTODIODE_DAYLIGHT);
  Serial.printf("photodiodeDaylight %i \n", photodiodeDaylight);

  // Open coors if  there is daylight
  if((millis() - checkDawnTimer) > 1000) {
  //   if (isDawn() && !isOuterDoorOpen() && !isInnerDoorOpen()) {
    if (photodiodeDaylight > 30) {
      Serial.printf("Line 189: checkDawnTimer %i \n", checkDawnTimer);
      openOuterDoor();
      openInnerDoor(); // Done
    // TODO - determine way to keep track of time for dawn and dusk, as this time changes through the year.
    }
    checkDawnTimer = millis();

    // Test starting point
    delay(3000);
    Serial.printf("Line 198: cose door so we can test again :)\n ");

  }

////// ------------------------- Outer door safety ------------------------- //////

  photodiodeDoor1 = analogRead(PHOTODIODE_DOOR1);
  if (photodiodeDoor1 > 50) {
    // Open door so duck doesn't get squished!
    openOuterDoor();
  }
  // Serial.printf("photodiodeDoor1 %i \n", photodiodeDoor1);

////// ------------------------- Inner door Safety ------------------------- //////

  photodiodeDoor2 = analogRead(PHOTODIODE_DOOR2);
  if (photodiodeDoor2 > 50) {
    openInnerDoor();
  }
  // Serial.printf(" %i \n", photodiodeDoor2);
}

////// ------------------------- OPEN doors ------------------------- //////

// TODO - finish getting this working
void openOuterDoor() {
  int switchStopOpeningDoor1 = digitalRead(SWITCH_STOP_OPENING_DOOR1);
  Serial.printf("switchStopOpeningDoor1 %d \n", switchStopOpeningDoor1);

  digitalWrite(DIR_DOOR1, HIGH);
  if (!switchStopOpeningDoor1) {
    for(int x = 0; x < steps; x++) {
      digitalWrite(STEP_DOOR1, HIGH);
      delay(2);
      digitalWrite(STEP_DOOR1, LOW);
      delay(2);
    }
  }
};

// Done?
void openInnerDoor() {
  int switchStopOpeningDoor2 = digitalRead(SWITCH_STOP_OPENING_DOOR2);
  Serial.printf("switchStopOpeningDoor2 %d \n", switchStopOpeningDoor2);

  digitalWrite(DIR_DOOR2, LOW);
  if (!switchStopOpeningDoor2) {
    for(int x = 0; x < steps; x++) {
      digitalWrite(STEP_DOOR2, HIGH);
      delay(2);
      digitalWrite(STEP_DOOR2, LOW);
      delay(2);
    }
  }
};

////// ------------------------- CLOSE doors ------------------------- //////

// Done?
void closeOuterDoor() {
  int switchStopClosingDoor1 = digitalRead(SWITCH_STOP_CLOSEING_DOOR1);
  Serial.printf("switchStopClosingDoor1 %d \n", switchStopClosingDoor1);

  digitalWrite(DIR_DOOR1, LOW);
  if (!switchStopClosingDoor1) {
    for(int x = 0; x < steps; x++) {
      digitalWrite(STEP_DOOR1, HIGH);
      delay(2);
      digitalWrite(STEP_DOOR1, LOW);
      delay(2);
    }
  }
};

// TODO - finish getting this working
void closeInnerDoor() {
  int switchStopClosingDoor2 = digitalRead(SWITCH_STOP_CLOSEING_DOOR2);
  Serial.printf("switchStopClosingDoor2 %d \n", switchStopClosingDoor2);

  digitalWrite(DIR_DOOR2, LOW);
  if (!switchStopClosingDoor2) {
    for(int x = 0; x < steps; x++) {
      digitalWrite(STEP_DOOR2, HIGH);
      delay(2);
      digitalWrite(STEP_DOOR2, LOW);
      delay(2);
    }
  }
};

bool isOuterDoorOpen() {
  ////// ------------------------- Door 1 / Outer Door ------------------------- //////
  // HALLEFFECT_DOOR1
  // hallEffectReadingDoor1 = digitalRead(HALLEFFECT_DOOR1);
  // if (hallEffectReadingDoor1) {
    
  // }
  // Test return value true for now. TODO - use halleffect sensor to check if door is open.
  return false;
};

bool isInnerDoorOpen() {
  ////// ------------------------- Door 2 / Inner Door ------------------------- //////
  // HALLEFFECT_DOOR2

    // Test return value true for now. TODO - use Halleffect sensor to check if door is open.
    return false;
};
