#include <Adafruit_DRV2605.h>

/*
 * created by Rui Santos, https://randomnerdtutorials.com
 * 
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
 */

#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

// Setting pin locations and variables
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
int trigPin2 = 9;    // Trigger for back sensor
int echoPin2 = 10;   // Echo for back sensor
long duration, cm, inches, speed;

// Setting vibrating effects for front distance sensor
uint8_t frontEffectClose = 7;
uint8_t frontEffectMedium = 8;
uint8_t frontEffectFar = 9;

// Setting vibrating effects for rear distance sensor
uint8_t rearEffectClose = 1;
uint8_t rearEffectMedium = 2;
uint8_t rearEffectFar = 3;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);

  //DRV setup
  drv.begin();
  drv.selectLibrary(1);

  //I2C trigger by sending 'go' command
  //default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
  
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

// Setting both sensors to run at the same time
void loop() {

  frontSensor();
  rearSensor();
}

// Front Sensor
void frontSensor() {
  
  // Set up sensor
  sensorConfig(trigPin, echoPin);

  // Test if object is within range
  if(cm <= 100) {
    playVibration(frontEffectClose, "7 - Soft Bump - 100%");
  }
  else if(cm <= 200 && cm > 100) {
    playVibration(frontEffectMedium, "8 - Soft Bump - 60%");
  }
  else if(cm <= 300 && cm > 200) {
    playVibration(frontEffectFar, "9 - Soft Bump - 30%");
  }
  else {
    Serial.print(inches);
    Serial.print("in front, ");
    Serial.print(cm);
    Serial.print("cm front");
    Serial.println();
  }
 
  delay(100);
}

// Rear Sensor
void rearSensor() {

  // Set up sensor
  sensorConfig(trigPin2, echoPin2);

  // Test if object is within range  
  if(cm <= 100) {
    playVibration(rearEffectClose, "1 - Strong CLick - 100%");
  }
  else if(cm <= 200 && cm > 100) {
    playVibration(rearEffectMedium, "2 - Strong Click - 60%");
  }
  else if(cm <= 300 && cm > 200) {
    playVibration(rearEffectFar, "3 - Strong Click - 30%");
  }
  else {
    Serial.print(inches);
    Serial.print("in rear, ");
    Serial.print(cm);
    Serial.print("cm rear");
    Serial.println();
  }
  
  delay(100);
}

void playVibration(uint8_t effect, String effectName) {
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    Serial.println(effectName);

    // set the effect to play
    drv.setWaveform(0, effect);  // play effect 
    drv.setWaveform(1, 0);       // end waveform

    // play the effect!
    drv.go();

    // wait a bit
    delay(cm*4); // time (in milliseconds) = distance / velocity
}

void sensorConfig(int trigger, int echo) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135

  // Convert time and distance into speed
  speed = cm / duration / 1000; // centimeters/milliseconds
}
