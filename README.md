# ESP01 Washer Machine simple controller
### A simple solution for controlling my washer machine

![Logo](https://raw.githubusercontent.com/ladbsoft/ESP01_Washer_Machine_simple_controller/master/Logo.png "Logo")

## About this
This projects satisfies one of my everyday needs: Alerts for when my clothes are done washing. Tired of checking every some minutes to know if my basic washer machine has done the laundry, I created this code, which loaded into an ESP-01 board checks if the door lock has been released and communicates to my IoT server to notify me through an app notification.

## Installation
You'll need:

### Hardware
- ESP-01 or similar microcontroller from the ESP8266 family
- A way of connecting the microcontroller to your washer machine. In my case, I used [this schematic](https://github.com/ladbsoft/ESP01_Washer_Machine_simple_controller/blob/master/Schematic.png)


### Software
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- The [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) library
- The [PubSubClient](https://github.com/knolleary/pubsubclient) library
- The [ESP8266 WiFiManager](https://github.com/tzapu/WiFiManager) library
- An MQTT server to communicate to and a way of notifying to your devices (in my case I used [OpenHAB](https://www.openhab.org/) and [PushBullet](https://www.pushbullet.com/)

### Configuration of the Arduino Sketch
You'll need to configure in Configuration.h the IP, user, and password of your MQTT server, the client ID, and the two MQTT topics to use and an SSID and password to create a new WiFi connection with a captive portal, that lets you connect to your WiFi router. This is not your actual WiFi router SSID and password, but a new one. Once you power on the microcontroller for the first time, connect to the SSID you configured here with your password and navigate to the captive portal to then configure your real WiFi connection.

More info (only in Spanish for now) in https://elprogramadoriberico.es/2019/04/22/haciendo-inteligente-mi-lavadora-parte-1/
