/* 
 * Project: IoT Duck Coop
 * Author: Squiggy Rubio
 * Description: 
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);


const int DIR_DOOR1 = D3;
const int STEP_DOOR1 = D4;

const int DIR_DOOR2 = D5;
const int STEP_DOOR2 = D6;

const int IFRED_DOOR1 = A0;
const int IFRED_DOOR2 = A1;

const int PHOTODIODE_DOOR1 = A2;
const int PHOTODIODE_DOOR2 = A5;

const int PHOTODIODE_DAYLIGHT = A3;

const int microMode = 1; // microstep mode, default is 1/16 so 16; ex: 1/4 would be 4
// full rotation * microstep divider
const int steps = 200 * microMode;

int photodiodeDaylight;

int photodiodeDoor1;
int photodiodeDoor2;

void setup()
{
  // setup step and dir pins as outputs
  pinMode(DIR_DOOR1, OUTPUT);
  pinMode(STEP_DOOR1, OUTPUT);

  pinMode(DIR_DOOR2, OUTPUT);
  pinMode(STEP_DOOR2, OUTPUT);

  pinMode(IFRED_DOOR1, OUTPUT);
  pinMode(IFRED_DOOR2, OUTPUT);

  pinMode(PHOTODIODE_DOOR1, INPUT);
  pinMode(PHOTODIODE_DOOR2, INPUT);

  pinMode(PHOTODIODE_DAYLIGHT, INPUT);

}

void loop()
{
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

  
  photodiodeDaylight = analogRead(PHOTODIODE_DAYLIGHT);
  // Serial.printf("photodiodeDaylight %i \n", photodiodeDaylight);

  // Door one (outer door)
  photodiodeDoor1 = analogRead(PHOTODIODE_DOOR1);
  if (photodiodeDoor1 > 50) {
    // Open door so duck doesn't get squished!
  }
  // Serial.printf("photodiodeDoor1 %i \n", photodiodeDoor1);

  // Door two (inner door)
  photodiodeDoor2 = analogRead(PHOTODIODE_DOOR2);
  if (photodiodeDoor1 > 50) {
    // Open door so duck doesn't get squished!
  }
  // Serial.printf(" %i \n", photodiodeDoor2);

}
