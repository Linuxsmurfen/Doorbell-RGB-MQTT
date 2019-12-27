# Doorbell-RGB-MQTT

Wifi connected doorbell using a esp8266




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
