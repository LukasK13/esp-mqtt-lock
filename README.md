# ESP8266 MQTT Lock Using JSON for Home Assistant

This project adds an easy way to create DIY lock for [Home Assistant](https://home-assistant.io/), an amazing, extensible, open-source home automation system.

## Installation/Configuration

To set this system up, you need to configure the [MQTT lock](https://www.home-assistant.io/components/lock.mqtt/) component in Home Assistant and set up a lock to control. This guide assumes that you already have Home Assistant set up and running. If not, see the installation guides [here](https://home-assistant.io/getting-started/).

### The Home Assistant Side
1. In your `configuration.yaml`, add the following, depending on the supported features of the lock:

    ```yaml
    - platform: mqtt
      name: frontdoor
      state_topic: "home-assistant/frontdoor"
      command_topic: "home-assistant/frontdoor/set"
      optimistic: false
      qos: 0
      retain: true
      value_template: '{{ value.x }}'
    ```
2. Set the `name`, `state_topic`, and `command_topic` to values that make sense for you.
3. Restart Home Assistant. Depending on how you installed it, the process differs. For a Raspberry Pi All-in-One install, use `sudo systemctl restart home-assistant.service` (or just restart the Pi).

### The Lock Side
I'm using ESP8266-01 microcontrollers for my locks because they are so cheap and small. The downside of the size and price is that programming them can be a bit of a hassle. There are many sites that go into detail, so I won't do it here. You'll need an ESP set up to work with the Arduino IDE. See the readme [here](https://github.com/esp8266/Arduino) for instructions. Another good device to work with is the [Wemos D1 Mini](https://wiki.wemos.cc/products:d1:d1_mini), which has a built-in micro-USB port and is much easier to program.

1. Using the Library Manager in the Arduino IDE, install [PubSubClient](http://pubsubclient.knolleary.net/). You can find the Library Manager in the "Sketch" menu under "Include Library" -> "Manage Libraries..."
2. Open the `mqtt_esp8266_light` project in the Arduino IDE.
3. Update the `config-sample.h` file with your settings for Lock type, pin numbers, WiFi settings, and MQTT settings.
  1. Ensure that the `CONFIG_MQTT_CLIENT_ID` setting is a unique value for your network.
  2. Set `CONFIG_MQTT_TOPIC_STATE` and `CONFIG_MQTT_TOPIC_SET` to match the values you put in your `configuration.yaml`.
4. Save the configuration file as `config.h`.
5. Open the `.ino` file in the Arduino IDE and upload to an ESP with the correct connections.
