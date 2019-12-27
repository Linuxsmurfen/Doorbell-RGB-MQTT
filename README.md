# Doorbell-RGB-MQTT

Wifi connected doorbell using a esp8266




## A mqtt message is published when the button is pressed
Default topic "doorbell/button"

-1   Long click time

 1   Short click time
 
 2   Short double click
 
 3   Short trippe click
 

## Set the RGB led via mqtt message topics

"doorbell/led/R"        Set red led between 0-1023 (0=off)

"doorbell/led/G"        Set green led between 0-1023 (0=off)

"doorbell/led/B"        Set blue led between 0-1023 (0=off)

"doorbell/led/Freq"     Set blink freq between 0-5000  (0=off)
