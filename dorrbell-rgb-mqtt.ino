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

const char* ssid = "........";
const char* password = ".........";

const char* mqtt_server = "............";
const char* mqttput = "doorbell/button";
const char* mqttget = "doorbell/led/#";

const int buttonPin = 12;     // the number of the pushbutton pin
const int redLed = 13;        // the number of the LED pin
const int greenLed = 14;      // the number of the LED pin
const int blueLed = 16;       // the number of the LED pin


//--------- DO NOT CHANGE BELOW -------------------------------
                                                                                            
//ClickButton button1(buttonPin, LOW, CLICKBTN_PULLUP);
ClickButton button1(buttonPin, HIGH);
//int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[5];

int ledState = 0;
int redState =   1023-(4*0);
int greenState = 1023-(4*0);
int blueState =  1023-(4*128);


//---- Wifi setup -----------------
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




//---- Reconnect -----------------
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
      
      // ... and re-subscribe
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





//---- Setup everything -----------------
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
  button1.multiclickTime = 350;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register
}





//---- Parse the mqtt message -------------
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

  // Set RGB, hardcoded for now
  ledState = 1;
  redState =   1023-(4*0);
  greenState = 1023-(4*0);
  blueState =  1023-(4*128);
}




//---- The main loop -----------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Update and check button state
  button1.Update();
  if (button1.clicks != 0) {
    snprintf (msg, 5, "%ld", button1.clicks);
    Serial.print("Button: ");
    Serial.println(msg);
    client.publish(mqttput, msg);
  }

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
  //for(int dutyCycle = 0; dutyCycle < (255*4); dutyCycle++){   
  //  // changing the LED brightness with PWM
  //  analogWrite(blueLed, dutyCycle);
  //  delay(2);
  //}

  // decrease the LED brightness
  //for(int dutyCycle = (255*4); dutyCycle > 0; dutyCycle--){
  //  // changing the LED brightness with PWM
  //  analogWrite(blueLed, dutyCycle);
  //  delay(0.5);
  //}

  analogWrite(redLed, redState);
  analogWrite(greenLed, blueState);
  analogWrite(blueLed, greenState);
 
}
