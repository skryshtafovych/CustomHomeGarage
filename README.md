# CustomHomeGarage
Simple app that will write status of Sensors and Have some Smart Logic to make it a smart Garage ;) since it will have control over Physical Door.
Often, you don't need to know the state of a digital input all the time, but
you just need to know when the input changes from one state to another.
For example, you want to know when a Hall Sensor goes from OFF to ON. This is called
state change detection, or edge detection


The circuit:
- hallSensor attached to pin 2 from +5V
- Relay attached from pin 5 to ground
- DHT11 Temp/Humidity Sensor attached pin 4

https://www.circuito.io/app?components=513,360216,653574,3061987

## TO-DO


- Add Timer to Loop to be able to set Specific Time For Certain Firebase Updates that change Often but stay at same equilibrium
- Will add Android app Part And simple none extensible security.
- Needs to just report info and be able to simply respond to a Boolean Firebase Value the Logic shall reside on Raspi that will also live locally on network. Which will be the only device with access to the Actual api resource and be blacklisted manually within the network.
- Create Diagram where the Logic of the app will live and how it will interact with rest of Samrt Home. This is just a garage sexample but logic for all smart things will be extrapilated into this Device/NET.

## WIRING TODO

- Garage + wire needs solder point passed pull down trigger.
- D7=>Servo


## REQUIRES the following Arduino libraries:
 - OLED Screen Drivers for ESP8266: https://github.com/ThingPulse/esp8266-oled-ssd1306
 - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
 - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
 - Firebase Arduino Lib: https://github.com/googlesamples/firebase-arduino/archive/master.zip
 - Json for Firebase http://downloads.arduino.cc/libraries/github.com/bblanchon/ArduinoJson-5.13.2.zip

## REQUIRES the following Board libraries:
 - ESP8266 WIFI Chipset Board: https://github.com/esp8266/Arduino
