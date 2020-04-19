// +------------------------------------------------------------------+
// |                       C O N S T A N T S                          |
// +------------------------------------------------------------------+

//MQTT
const char* mqttServer     = "<YourMQTTServerIP>";
const int   mqttPort       = 1883;
const char* mqttUser       = "<YourMQTTServerUser>";
const char* mqttPassword   = "<YourMQTTServerPassword>";
const char* mqttClientId   = "<ANewMQTTClientID>";
const char* mqttDoorTopic  = "<ANewMQTTTopic>";
const char* mqttStartTopic = "<ANewMQTTTopic2>";

// GPIO
const byte  startCyclePin  = 3;
const byte  doorLockPin    = 1;

// WI-FI CONFIGURATION NETWORK. IF THE ESP01 CANNOT CONNECT TO THE
// LAST WI-FI SPOT USED, A NEW CONNECTION WITH THIS SSID AND
// PASSWORD IS CREATED ON THE ESP01, SO YOU CAN CONNECT TO IT AND
// CONFIGURE THE NEW CONNECTION
const char* wifiSsid         = "<NewSSID>";
const char* wifiPassword     = "<NewPassword>";
