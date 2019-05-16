/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a Hall Sensor goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - hallSensor attached to pin 2 from +5V
  - Relay attached from pin 5 to ground

  created  27 Sep 2005 -by Tom Igoe
  modified 30 Aug 2011 -by Tom Igoe
  modified 09 May 2019 -by Stepan Kryshtafovych



  You Can find modified example on Github.
  https://github.com/skryshtafovych/CustomHomeGarage

  This was based on below example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/ButtonStateChange
*/
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "secretValues.h"





// this constant won't change:
const int  hallSensorPin = 2;    // the pin that the pushbutton is attached to
const int relayPin = 5;       // the pin that the LED is attached to

// Variables will change:
int hallSensorCounter = 0;   // counter for the number of button presses
int hallSensorState = 0;         // current state of the button
int lastHallSensorState = 0;     // previous state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(hallSensorPin, INPUT);
  // initialize the LED as an output:
  pinMode(relayPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // connect to wifi.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);



}


void loop() {
  // read the pushbutton input pin:
  hallSensorState = digitalRead(hallSensorPin);

  // compare the hallSensorState to its previous state
  if (hallSensorState != lastHallSensorState) {
    // if the state has changed, increment the counter
    if (hallSensorState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      hallSensorCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(hallSensorCounter);
      Firebase.setString("hallSensorGarageH", "true");

    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
      Firebase.setString("hallSensorGarageH", "false");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastHallSensorState = hallSensorState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (hallSensorCounter % 2 == 0) {
    digitalWrite(relayPin, HIGH);
      Firebase.setString("hallSensorGarage", "true");
  } else {
    digitalWrite(relayPin, LOW);
          Firebase.setString("hallSensorGarage", "false");

  }

}
