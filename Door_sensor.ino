#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "";
const char* password = "";
String serverName = "";

const int door_sensor = D1;
int oldState;
int doorState;

void setup() {
  Serial.begin(115200);
  pinMode(door_sensor, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

 
  doorState = digitalRead(door_sensor);
}

void loop() {
  oldState = doorState;
  doorState = digitalRead(door_sensor);
  String txt = "";
  if (doorState == HIGH && oldState == LOW) {
    Serial.println("Door Open");
    txt = "?door_status=door_open";
    senData(txt);
  } else if (doorState == LOW && oldState == HIGH) {
    Serial.println("Door Close");
    txt = "?door_status=door_close";
    senData(txt);
  }
}

void senData(String txt) {
  Serial.println(txt);
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String serverPath = serverName + txt;
    Serial.println(serverPath);
    http.begin(client, serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}