#include <SoftwareSerial.h>   
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define pinEcho D8
#define pinTrig D7
#define pinLed D0
#define pinBuzzer D5
// Replace with your network credentials
const char* ssid     = "iPhone";
const char* password = "12345678"; 

WiFiClient wifiClient;

long durasi, jarak;

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  durasi = pulseIn(pinEcho, HIGH); // Use pinEcho instead of pinTrig
  int jarak = (durasi / 2) / 29.1;
  Serial.print("Jarak : ");
  Serial.print(jarak);
  Serial.println(" cm");

  if (jarak < 20){
    digitalWrite(pinLed, HIGH);
    digitalWrite(pinBuzzer, HIGH);
    delay(100);
    digitalWrite(pinBuzzer, LOW);
    delay(100);
    digitalWrite(pinBuzzer, HIGH);
    delay(100);
    digitalWrite(pinBuzzer, LOW);
    delay(100);
  }
  else {
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinLed, LOW);
  }

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
    WiFiClient wifiClient;

    String address;
    address = "http://172.20.10.3/ultrasonic/data.php?cm=";
    address += String(jarak);

    http.begin(wifiClient, address);  //Specify request destination
    
    int httpCode = http.GET();                                  //Send the request
    String payload;  
    if (httpCode > 0) { //Check the returning code
      
        payload = http.getString();   //Get the request response payload
        payload.trim();
        if( payload.length() > 0 ){
           Serial.println(payload + "\n");
        }             //Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }else{
    Serial.print("Not connected to wifi ");Serial.println(ssid);
  }
  delay(500);
}
