/*
 * ESP8266 MQTT Lock for Home Assistant.
 *
 * Created DIY locks for Home Assistant using MQTT and JSON.
 *
 * Copy the included `config-sample.h` file to `config.h` and update
 * accordingly for your setup.
 *
 * See https://github.com/corbanmailloux/esp-mqtt-rgb-led for more information.
 */

// Set configuration options for pins, WiFi, and MQTT in the following file:
#include "config.h"

#include <ESP8266WiFi.h>

// http://pubsubclient.knolleary.net/
#include <PubSubClient.h>

bool command_unlock = false;
unsigned long unlockStartTime = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(CONFIG_PIN_LOCK, OUTPUT);

  // Set the LED_BUILTIN based on the CONFIG_LED_BUILTIN_MODE
  switch (CONFIG_LED_BUILTIN_MODE) {
    case 0:
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case 1:
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    default: // Other options (like -1) are ignored.
      break;
  }

  if (CONFIG_DEBUG) {
    Serial.begin(115200);
  }

  setup_wifi();
  client.setServer(CONFIG_MQTT_HOST, CONFIG_MQTT_PORT);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(CONFIG_WIFI_SSID);

  WiFi.mode(WIFI_STA); // Disable the built-in WiFi access point.
  WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message_terminated[length + 1];
  for (int i = 0; i < length; i++) {
    message_terminated[i] = (char)payload[i];
  }
  message_terminated[length] = '\0';
  Serial.println(message_terminated);

  if (strcmp(message_terminated, CONFIG_MQTT_PAYLOAD_LOCK) == 0) {
    Serial.println("Lock command received");
    command_unlock = false;
  } else if (strcmp(message_terminated, CONFIG_MQTT_PAYLOAD_UNLOCK) == 0) {
    Serial.println("Unlock command received");
    command_unlock = true;
    unlockStartTime = 0;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CONFIG_MQTT_CLIENT_ID, CONFIG_MQTT_USER, CONFIG_MQTT_PASS)) {
      Serial.println("connected");
      client.publish(CONFIG_MQTT_TOPIC_STATE, "LOCK", true);
      client.subscribe(CONFIG_MQTT_TOPIC_SET);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (!client.loop()) {
    reconnect();
  }
  
  client.loop();
  if (command_unlock) {
    if (unlockStartTime == 0) {
      unlockStartTime = millis();
      digitalWrite(CONFIG_PIN_LOCK, HIGH);
      Serial.println("Door nlocked");
      client.publish(CONFIG_MQTT_TOPIC_STATE, "UNLOCK", true);
    } else {
      if ((millis() - unlockStartTime) >= CONFIG_UNLOCK_TIME) {
        unlockStartTime = 0;
        command_unlock = false;
        digitalWrite(CONFIG_PIN_LOCK, LOW);
        Serial.println("Door locked");
        client.publish(CONFIG_MQTT_TOPIC_STATE, "LOCK", true);
      }
    }
  }
}
