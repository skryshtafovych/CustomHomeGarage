/*
  Cloud State change detection (cloud edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a Hall Sensor goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a hall Sensor or button changes from off to on
  and on to off or Up to Down or Down to Up ...

  The circuit:
  - hallSensor attached to pin 2 from +5V
  - Relay attached from pin 5 to ground
  - DHT11 Temp/Humidity Sensor attached pin 4

  created  27 Sep 2005 -by Tom Igoe
  modified 30 Aug 2011 -by Tom Igoe
  modified 09 May 2019 -by Stepan Kryshtafovych

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// - Firebase Arduino Lib: https://github.com/googlesamples/firebase-arduino/archive/master.zip
// - Json for Firebase http://downloads.arduino.cc/libraries/github.com/bblanchon/ArduinoJson-5.13.2.zip



  You Can find modified example on Github.
  https://github.com/skryshtafovych/CustomHomeGarage

  This was based on below example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/ButtonStateChange
*/
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "secrets.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <EEPROM.h>


#include "SSD1306.h"
#include "SH1106.h"
#include <Servo.h>


extern "C" {
#include "user_interface.h"
}


//////////////////////////
//Pin DEFINITIONS////////
////////////////////////


#define DHTPIN 12    // Digital pin connected to the DHT sensor  D6
SSD1306 display(0x3c, 5, 4);   // GPIO 5 = D1, GPIO 4 = D2
#define ledPin 2       // led pin ( 2 = built-in LED)
#define btn D3         // GPIO 0 = FLASH BUTTON
// this constant won't change:
const int  hallSensorPin = 2 ;    // the pin that the pushbutton is attached to D4
const int relayPin = 14;       // the pin that the LED is attached to D5
Servo myservo;  // create servo object to control a servo THIS WE HAVE TO SET YO








#define DHTTYPE    DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;


/*===== SETTINGS =====*/
/* create display(Adr, SDA-pin, SCL-pin) */
//SH1106 display(0x3c, 5, 4);

/* select the button for your board */

#define maxCh 13       // max Channel -> US = 11, EU = 13, Japan = 14
#define packetRate 5   // min. packets before it gets recognized as an attack

#define flipDisplay true

/* Display settings */
#define minRow       0              /* default =   0 */
#define maxRow     127              /* default = 127 */
#define minLine      0              /* default =   0 */
#define maxLine     63              /* default =  63 */

/* render settings */
#define Row1         0
#define Row2        30
#define Row3        35
#define Row4        80
#define Row5        85
#define Row6       125

#define LineText     0
#define Line        12
#define LineVal     47

//===== Run-Time variables =====//
unsigned long prevTime   = 0;
unsigned long curTime    = 0;
unsigned long pkts       = 0;
unsigned long no_deauths = 0;
unsigned long deauths    = 0;
int curChannel           = 1;
unsigned long maxVal     = 0;
double multiplicator     = 0.0;
bool canBtnPress         = true;

unsigned int val[128];

unsigned long timeDude;

// Variables will change:
int hallSensorCounter = 0;   // counter for the number of button presses
int hallSensorState = 0;         // current state of the button
int lastHallSensorState = 0;     // previous state of the button



int pos = 0;    // variable to store the servo position 


void setup() {
  // initialize the button pin as a input:
  pinMode(hallSensorPin, INPUT);
  // initialize the relay as an output:
  pinMode(relayPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // connect to wifi.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

    display.init();
  if (flipDisplay) display.flipScreenVertically();

  /* show start screen */
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Garage-");
  display.drawString(0, 16, "Station");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 40, "stepank.com");
  display.drawString(0, 50, "Stepan Kryshtafovych");
  display.display();

  delay(2500);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


      // Initialize device.
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
   // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.println(F("------------------------------------"));


  myservo.attach(13);  // attaches the servo on pin 9 to the servo object 

  



}

void feedCAT(){
    myservo.attach(13);  // attaches the servo on pin 9 to the servo object 

   for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
    myservo.detach();  // attaches the servo on pin 9 to the servo object 

  }


void loop() {

  timeDude = millis();
  FirebaseObject fire = Firebase.get("/");
  String temp = fire.getString("upTimeGarage");
  String relayActor = fire.getString("garageRelayActor");
  if(relayActor.equals("Open")){
        digitalWrite(ledPin, HIGH);

    digitalWrite(relayPin, LOW);
  Serial.println("Inside LOW : "+temp+relayActor);

    
    }
      if(relayActor.equals("Close")){
        digitalWrite(ledPin, LOW);

        digitalWrite(relayPin, HIGH);
      }

  Serial.println("Firebase value: "+temp+relayActor);

  /* show start screen */
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 40, "upTimer");
  display.drawString(0, 50, (String)timeDude);
  display.display();
  Firebase.setString("hallSensorGarage", "true");
  Firebase.setString("upTimeGarage", (String)timeDude);




  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Firebase.setInt("GarageT", event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Firebase.setInt("GarageH", event.relative_humidity);
    Serial.println(F("%"));
    Serial.println(Firebase.getString("hallSensorGarageH"));

  }


  String catFeeder = fire.getString("catFeederActor");
  if(catFeeder.equals("Snack")){

 feedCAT();
    
    }





  



  delay(10000);



}
