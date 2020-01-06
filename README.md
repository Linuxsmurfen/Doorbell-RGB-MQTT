# Doorbell-RGB-MQTT

Wifi connected doorbell using a esp8266.
Uses mqtt to update led status and notify when button is pressed.
It has dual switches so one will be used to replace the existing button while the other will be connected to the esp8266.
Supports OTA (Over the Air) update for uploading the firmware.


![Doorbell picture](/doorbell-rgb-mqtt.jpg)



## A mqtt message is published when the button is pressed
Default topic "doorbell/button"

| Value | Description |
| :---: | --- |
| Clicked:-1 | Long click |
| Clicked:1 | Short click |
| Clicked:2 | Short double click |
| Clicked:3 | Short triple click |
| Last:-1,2,3 | Last three clicks |


## Set the RGB led via mqtt message topics

| Topic | Description | Default |
| --- | --- | :---: |
| doorbell/led/rgb | Set the RGB led 0-255,0-255,0-255 (0=off) | 255,0,125 |
| doorbell/led/Freq | Set blink freq between 0-5000  (0=off) | 2000 |
| doorbell/led | ON, OFF, fixed, flash | ON |


## Integration with Home Assistant
![homeassistant picture](/homeassistant.png)

Example: 
- Button pressed for a long time
~~~
binary_sensor:
  - platform: mqtt
    name: "Doorbell"
    state_topic: "doorbell/button"
    payload_on: "Clicked:-1"
    off_delay: 10
    device_class: occupancy
~~~

- Set the color
~~~
light:
  - platform: mqtt
    name: "Doorbell"
    command_topic: "doorbell/led"
    rgb_command_topic: "doorbell/led/rgb"
    white_value_command_topic: "doorbell/led/Freq"
    white_value_scale: 5000
    effect_command_topic: "doorbell/led"
    effect_list: [fixed,flash]
~~~

## Bill of materials

| Description | Link |
| --- | --- |
| Rugged Metal Pushbutton - 22mm 6V RGB Momentary | https://www.adafruit.com/product/3423 |
| Adafruit HUZZAH ESP8266 Breakout | https://www.adafruit.com/product/2471 |
| Resistor - 150Ω | |
| Resistor - 470kΩ | |


## Schematic

![schematic picture](/Schematic_doorbell-rgb-mqtt.png)
