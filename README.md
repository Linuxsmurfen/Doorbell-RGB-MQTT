# Doorbell-RGB-MQTT

Wifi connected doorbell using a esp8266.
Uses mqtt to update led status and notify when button is pressed

![Doorbell picture](/doorbell-rgb-mqtt.jpg)



## A mqtt message is published when the button is pressed
Default topic "doorbell/button"

| Value | Description |
| :---: | --- |
| -1 | Long click |
| 1 | Short click |
| 2 | Short double click |
| 3 | Short triple click |


## Set the RGB led via mqtt message topics

| Topic | Description | Default |
| --- | --- | :---: |
| doorbell/led/R | Set red led between 0-1023 (0=off) | 1023 |  
| doorbell/led/G | Set green led between 0-1023 (0=off) | 0 |
| doorbell/led/B | Set blue led between 0-1023 (0=off) | 500 |
| doorbell/led/Freq | Set blink freq between 0-5000  (0=off) | 2000 |


## Bill of materials

| Description | Link |
| --- | --- |
| Rugged Metal Pushbutton - 22mm 6V RGB Momentary | https://www.adafruit.com/product/3423 |
| Adafruit HUZZAH ESP8266 Breakout | https://www.adafruit.com/product/2471 |
| Resistor - 150Ω | |
| Resistor - 470kΩ | |


## Schematic

![schematic picture](/Schematic_doorbell-rgb-mqtt.png)
