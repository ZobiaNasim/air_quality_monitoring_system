#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Arduino.h>
#include<ArduinoOTA.h>



#define WIFI_NETWORK  "vivo Y21T"
#define WIFI_PASSWORD "zobia2004"
#define WIFI_TIMEOUT_MS 20000

void connectToWiFi(){
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK ,WIFI_PASSWORD);
  unsigned long startAttemptTime= millis();
  while (WiFi.status() != WL_CONNECTED && millis()-startAttemptTime< WIFI_TIMEOUT_MS)
  { Serial.print(".");
   delay(100);}
  if(WiFi.status()!=WL_CONNECTED){
    Serial.print("failed");
  }else{
    Serial.print("Connected");
    Serial.println(WiFi.localIP());
  }
}
void setup() {
  
Serial.begin(115200);
connectToWiFi();
ArduinoOTA.begin();

}
//handle ota
void loop() {
 ArduinoOTA.handle();
 

}
