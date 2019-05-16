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
## TO-DO

- Will add Android app Part And simple none extensible security.


## REQUIRES the following Arduino libraries:
 - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
 - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
 - Firebase Arduino Lib: https://github.com/googlesamples/firebase-arduino/archive/master.zip
 - Json for Firebase http://downloads.arduino.cc/libraries/github.com/bblanchon/ArduinoJson-5.13.2.zip
