// #==================================================================#
// ‖ Author: Luis Alejandro Domínguez Bueno (LADBSoft)                ‖
// ‖ Date: 2019-03-20                                    Version: 1.0 ‖
// #==================================================================#
// ‖ Name: ESP8266 MQTT Washing machine simple controller             ‖
// ‖ Description: A small sketch for the ESP8266 (ESP-01 to be exact) ‖
// ‖ for implementing a couple of functionalities I think any washing ‖
// ‖ machine should have: end-of-program alerts and time delay.       ‖
// ‖ My cheap washing machine had no way of telling me it had         ‖
// ‖ finished washing my clothes, nor any way of setting a time       ‖
// ‖ delay. That had to be changed :).                                ‖
// ‖                                                                  ‖
// ‖ With this sketch in a tiny ESP-01, the status of the door lock   ‖
// ‖ is transmitted to my home automation server using MQTT, so I can ‖
// ‖ create notifications in my phone to notice when the washing      ‖
// ‖ program has finished, and a command can be sent to start or      ‖
// ‖ pause the washing program.                                       ‖
// #==================================================================#
// ‖ Version history:                                                 ‖
// #==================================================================#
// ‖ 1.0:  Code cleanup. First stable version.                        ‖
// ‖ 0.4a: Disabled serial communication in order to use GPIO 1 and 3 ‖
// ‖ instead of 0 and 4, after confronting some issues.               ‖
// ‖ 0.3a: Basic door lock sense support added.                       ‖
// ‖ 0.2a: Basic cycle start support added.                           ‖
// ‖ 0.1a: Start of development. Connection to the MQTT server and    ‖
// ‖ communication tests.                                             ‖
// #==================================================================#

// +------------------------------------------------------------------+
// |                        I N C L U D E S                           |
// +------------------------------------------------------------------+
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Connection.h"

// +------------------------------------------------------------------+
// |                       C O N S T A N T S                          |
// +------------------------------------------------------------------+
const char* mqttClientId   = "GF_WashingMachine";
const char* mqttDoorTopic  = "Home/GF_Patio/WashingMachine/Door";
const char* mqttStartTopic = "Home/GF_Patio/WashingMachine/Start";
const byte  startCyclePin  = 3;
const byte  doorLockPin    = 1;

// +------------------------------------------------------------------+
// |                         G L O B A L S                            |
// +------------------------------------------------------------------+

WiFiClient espClient;
PubSubClient client(espClient);
long lastCheck = 0;
bool wasDoorLocked = false;
int doorUnlockedFor = 0;

// +------------------------------------------------------------------+
// |                           S E T U P                              |
// +------------------------------------------------------------------+

void setup() {
  //Disable Serial pins in order to use them as GPIO
  pinMode(1,FUNCTION_3); //TX
  pinMode(3,FUNCTION_3); //RX

  pinMode(startCyclePin, OUTPUT);   // For washing cycle start
  pinMode(doorLockPin, INPUT);      // For door lock detection

  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// +------------------------------------------------------------------+
// |                            L O O P                               |
// +------------------------------------------------------------------+

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  //Check status every 5 seconds
  if (now - lastCheck > 5000) {
    lastCheck = now;

    if (isDoorLocked()) {
      if (!wasDoorLocked) {
        wasDoorLocked = true;
        client.publish(mqttStartTopic, "ON");
      }
      
      client.publish(mqttDoorTopic, "CLOSED");

      if (doorUnlockedFor > 0) {
        doorUnlockedFor = 0;
      }
    } else {
      if (wasDoorLocked) {
        doorUnlockedFor++;
      }

      //Door needs to be unlocked for 10 seconds (2 checks). This is
      //done to avoid misunderstanding power fluctuations as
      //unlocking signals.
      if (doorUnlockedFor >= 2) {
        client.publish(mqttDoorTopic, "OPEN");

        client.publish(mqttStartTopic, "OFF");
        wasDoorLocked = false;
      }
    }
  }
}

// +------------------------------------------------------------------+
// |                     S U B R O U T I N E S                        |
// +------------------------------------------------------------------+

void setup_wifi() {
  delay(10);

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Add \0 to payload, to convert it to string later
  byte* payloadNull = (byte*)malloc(length+1);
  memcpy(payloadNull, payload, length);
  payloadNull[length] = '\0';

  if(String(topic).equals(String(mqttStartTopic))) {
    if (String((char*)payloadNull).equals("ON")) {
      startWashingCycle();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(mqttClientId, mqttUser, mqttPassword)) {
      // Once connected, resubscribe
      client.subscribe(mqttStartTopic);
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool isDoorLocked() {
  //Returns true if pin is high (door is locked); false otherwise
  return digitalRead(doorLockPin) == 1;
}

void startWashingCycle() {
  if(!isDoorLocked()) {
    //Presses the start button for 0.5 seconds
    digitalWrite(startCyclePin, HIGH);
    delay(500);
    //Releases the start button and waits for the washing cycle to start
    //(the door will lock after some time)
    digitalWrite(startCyclePin, LOW);
    while(!isDoorLocked()) {
      delay(1000);
    }
  }
}
