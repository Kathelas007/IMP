#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

#include <FS.h>
#include <Wire.h>

// ************ GLOBAL ****************************************************
// *** LEDS ***

const int N = 3;

typedef struct Led_struct {
  int nums[N];
  bool state[N];
  int on_s[3];
  int off_s[3];
} Led_struct;

Led_struct leds = { .nums = {4, 0, 2}, .state = {false, false, false}, .on_s = {HIGH, HIGH, LOW}, .off_s = {LOW, LOW, HIGH}};

// *** SERVER ***

AsyncWebServer server(80);

const char *ssid = "ESPAccesPoint";
const char *password = "esp123456789";

// *********** FUNCTIONS ************************************************

void handleRoot(AsyncWebServerRequest * request) {
  Serial.println("handling root");
  request->send(SPIFFS, "/data/index.html");
}

void handleLeds() {
}

void handleSequences() {
}

// *************** SETUP *************************************************

void setup() {
  // turn off leds
  for (int i = 0; i < N; i++) {
    pinMode(leds.nums[i], OUTPUT);
    digitalWrite(leds.nums[i], leds.off_s[i]);
  }

  // start serial
  delay(1000);
  Serial.begin(115200);

  // start SPIFFS (saved inde.xml)
  while (!SPIFFS.begin()) {
    delay(500);
  };
  Serial.print("SPIFFS is ready");

  // set acces point
  Serial.print("Setting access point... ");
  while (!WiFi.softAP(ssid, password)) {
    delay(500);
  }
  Serial.println("OK");

  IPAddress APIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(APIP);

  //start server
  //  server.on("/", HTTP_GET, handleRoot());
  //  server.on("/led", HTTP_GET, handleLeds());
  //  server.on("/seq", HTTP_GET, handleSequences());
  //  server.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("handling root");
    request->send(SPIFFS, "/index.html");
  });

  server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/bootstrap.bundle.min.js", "text/javascript");
  });
   
  server.on("/src/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/jquery-3.3.1.min.js", "text/javascript");
  });
   
  server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/bootstrap.min.css", "text/css");
  });

    server.on("/plain", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "Hello World");
  });

  
  server.begin();

  Serial.println("HTTP server is running\n");

}


// *************** LOOP *************************************************
void loop() {

}
