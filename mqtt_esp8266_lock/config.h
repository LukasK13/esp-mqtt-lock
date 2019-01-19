/*
 * This is a sample configuration file for the "mqtt_esp8266" lock.
 *
 * Change the settings below and save the file as "config.h"
 * You can then upload the code using the Arduino IDE.
 */

// Pins
#define CONFIG_PIN_LOCK 0 // For BRIGHTNESS and RGBW

// WiFi
#define CONFIG_WIFI_SSID "Wifi SSID"
#define CONFIG_WIFI_PASS "Wifi Password"

// MQTT
#define CONFIG_MQTT_HOST "MQTT Host"
#define CONFIG_MQTT_PORT 1883 // Usually 1883
#define CONFIG_MQTT_USER "esp01"
#define CONFIG_MQTT_PASS "MQTT Password"
#define CONFIG_MQTT_CLIENT_ID "ESP_Lock" // Must be unique on the MQTT network

// MQTT Topics
#define CONFIG_MQTT_TOPIC_STATE "home-assistant/frontdoor"
#define CONFIG_MQTT_TOPIC_SET "home-assistant/frontdoor/set"

#define CONFIG_MQTT_PAYLOAD_LOCK "LOCK"
#define CONFIG_MQTT_PAYLOAD_UNLOCK "UNLOCK"

// Unlock time in ms
#define CONFIG_UNLOCK_TIME 2000

// Set the mode for the built-in LED on some boards.
// -1 = Do nothing. Leave the pin in its default state.
//  0 = Explicitly set the LED_BUILTIN to LOW.
//  1 = Explicitly set the LED_BUILTIN to HIGH. (Off for Wemos D1 Mini)
#define CONFIG_LED_BUILTIN_MODE 0

// Enables Serial and print statements
#define CONFIG_DEBUG true
