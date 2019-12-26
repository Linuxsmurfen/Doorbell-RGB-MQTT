/*
 RGB doorbell with MQTT

 Use MQTT to set the RGB color on the button
 Notify via MQTT when button is pushed.

 Created by Linuxsmurfen
 Date: 2019-12-26

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ClickButton.h>

// Update these with values suitable for your network.

const char* ssid = ".......";
const char* password = ".............";

const char* mqtt_server = "test.mosquitto.org";
const char* mqttput = "doorbell/button";
const char* mqttget = "doorbell/led/#";

const int buttonPin = 12;     // the number of the pushbutton pin
const int redLed = 13;        // the number of the LED pin
const int greenLed = 14;      // the number of the LED pin
const int blueLed = 16;       // the number of the LED pin


//--------- DO NOT CHANGE BELOW -------------------------------
                                                                                            
ClickButton button1(buttonPin, LOW, CLICKBTN_PULLUP);
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[5];



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  if (topic == "doorbell/led/R") {
    Serial.print("test");
  }

  // LED RED
  if ((char)payload[0] == '1') {
    digitalWrite(redLed, LOW);
  } else {
    digitalWrite(redLed, HIGH);
  }

  // LED GREEN
  if ((char)payload[1] == '1') {
    digitalWrite(greenLed, LOW); 
  } else {
    digitalWrite(greenLed, HIGH);
  }

  // LED BLUE
  if ((char)payload[2] == '1') {
    digitalWrite(blueLed, LOW);
  } else {
    digitalWrite(blueLed, HIGH);
  }

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      client.publish(mqttput, "Connected!");
      
      // ... and resubscribe
      client.subscribe(mqttget);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Setup the pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);


  // Setup button timers (all in milliseconds / ms)
  button1.debounceTime   = 20;   // Debounce timer in ms
  button1.multiclickTime = 250;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //long now = millis();
  //if (now - lastMsg > 2000) {
  //  lastMsg = now;
  //  ++value;
  //  snprintf (msg, 50, "hello world #%ld", value);
  //  Serial.print("Publish message: ");
  //  Serial.println(msg);
  //  client.publish(mqttput, msg);
  //}



  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle < 1023; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(blueLed, dutyCycle);
    delay(1);
  }

  // decrease the LED brightness
  for(int dutyCycle = 1023; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(blueLed, dutyCycle);
    delay(1);
  }


  

  // Update button state
  button1.Update();
  value = button1.clicks;
  if(value != 0) {
    snprintf (msg, 5, "%ld", value);
    Serial.print("Button: ");
    Serial.println(msg);
    client.publish(mqttput, msg);
  }
  
}
