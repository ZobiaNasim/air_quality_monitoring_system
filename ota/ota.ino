#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include<ArduinoOTA.h>

#define WIFI_NETWORK  "vivo Y21T"
#define WIFI_PASSWORD "zobia2004"
#define WIFI_TIMEOUT_MS 20000

#define DHTPIN 21 
#define DHTTYPE DHT11
#define FIRMWARE_VERSION "v1.0.0"

DHT dht(DHTPIN, DHTTYPE);

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
  } else {
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  }
}

void sendDataToServer(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.66.78:4000/api/sensor"); 
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperature"] = temperature;
    jsonDoc["humidity"] = humidity;
    String jsonData;
    serializeJson(jsonDoc, jsonData);

    int httpResponseCode = http.POST(jsonData);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  dht.begin();
  ArduinoOTA.begin();
    Serial.print("Firmware Version: ");
  Serial.println(FIRMWARE_VERSION);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C  Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    sendDataToServer(temperature, humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(5000); 
  ArduinoOTA.handle(); // Send data every 5 seconds
}