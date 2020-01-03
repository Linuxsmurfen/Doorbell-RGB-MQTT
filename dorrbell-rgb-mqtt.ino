/*
 RGB doorbell with MQTT

 Use MQTT to set the RGB color on the button
 Notify via MQTT when button is pushed.

 Created by Linuxsmurfen
 Date: 2020-01-03

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ClickButton.h>      // https://github.com/marcobrianza/ClickButton
#include <arduino_secrets.h>  // Includes the SECRET_* stuff


// Update these with values suitable for your network.

const char* ssid = SECRET_SSID;
const char* password = SECRET_PW;

const char* mqtt_server = "192.168.1.200";
const char* mqttput =   "doorbell/button";
const char* mqttLed =   "doorbell/led";
const char* mqttRGB =   "doorbell/led/rgb";
const char* mqttFreq =  "doorbell/led/Freq";


const int buttonPin = 12;     // the number of the pushbutton pin
const int redLed = 13;        // the number of the LED pin
const int greenLed = 14;      // the number of the LED pin
const int blueLed = 16;       // the number of the LED pin


//--------- DO NOT CHANGE BELOW -------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
ClickButton button1(buttonPin, HIGH);
char msg[5];
double cosValue = 0;
int ledFreq = 2000;              // Blink freq 0-5000  (0=off)
int redState =   1023;           // Red light 0-1023 (0=off)
int greenState = 0;              // Green light 0-1023 (0=off)
int blueState =  500;            // Blue light 0-1023 (0=off)
boolean ledState = true;         // Led status (true=on)
const int highLed = 1023;        // Max analog value


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
      
      // ... and subscribe
      client.subscribe(mqttLed);
      client.subscribe(mqttFreq);
      client.subscribe(mqttRGB);
      
      
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
//Message arrived: [doorbell/led/freq] 2000
//Message arrived: [doorbell/led/rgb] 235,134,235

void callback(char* topic, byte* payload, unsigned int length) {
  
  // Print topic and payload
  Serial.print("Message arrived: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Convert payload to string and integer
  payload[length] = '\0';
  String s = String((char*)payload);
  int i = s.toInt();

  // Led Topic
  if (strcmp(topic,mqttLed)==0) {
    if (s == "ON" )  {ledState = true;  }
    if (s == "OFF")  {ledState = false; }
    if (s == "fixed") {ledFreq = 0; }
    if (s == "flash") {ledFreq = 300; }
  }
  
  // Freq Topic
  if (strcmp(topic,mqttFreq)==0) {
    if ((i>=0) && (i<5001)) { ledFreq = i; }
  }

  // RGB Topic 
  if (strcmp(topic,mqttRGB)==0) {
    char *p = (char*)payload;
    char *tok;
    tok = strtok(p, ",");
    redState = atoi(tok) * 4;
    tok = strtok(NULL, ",");
    if (tok != NULL) {
      greenState = atoi(tok) * 4;
      tok = strtok(NULL, ",");
      if (tok != NULL) {
          blueState = atoi(tok) * 4;
      }
    }
  }


  // Print values
  Serial.print("Red: "); Serial.print(redState); Serial.println();
  Serial.print("Green: "); Serial.print(greenState); Serial.println();
  Serial.print("Blue: "); Serial.print(blueState); Serial.println();
  Serial.print("Freq: "); Serial.print(ledFreq); Serial.println();
  Serial.print("Led: "); Serial.print(ledState); Serial.println();
}




//---- The main loop -----------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check button state
  button1.Update();
  if (button1.clicks != 0) {
    snprintf (msg, 5, "%ld", button1.clicks);
    Serial.print("Button: ");
    Serial.println(msg);
    client.publish(mqttput, msg);
  }

  // Update RGB led
  if (ledState) {
    if (ledFreq == 0) {
      analogWrite(redLed, highLed-redState);
      analogWrite(greenLed, highLed-greenState);
      analogWrite(blueLed, highLed-blueState);
    } else {
      cosValue = cos(2*PI/ledFreq*millis());
      analogWrite(redLed, (highLed-redState/2) + (redState/2) * cosValue);
      analogWrite(blueLed, (highLed-blueState/2) + (blueState/2) * cosValue);
      analogWrite(greenLed, (highLed-greenState/2) + (greenState/2) * cosValue);
    }
  } else {
      analogWrite(redLed, highLed);
      analogWrite(greenLed, highLed);
      analogWrite(blueLed, highLed);
  }

  
} 
