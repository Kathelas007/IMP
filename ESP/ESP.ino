#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

#include <AsyncJson.h>
#include <ArduinoJson.h>

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

bool is_sequence_running = false;

// *** SERVER ***

AsyncWebServer server(80);

const char *ssid = "ESPAccesPoint";
const char *password = "esp123456789";

// *********** FUNCTIONS ************************************************

void handleRoot(AsyncWebServerRequest * request) {
  Serial.println("handling root");
  request->send(SPIFFS, "/index.html");
}

void handleLeds(AsyncWebServerRequest * request) {
  if (request->params() != 2) return;

  int ledId = request->getParam(0)->value().toInt();
  int state = request->getParam(1)->value().toInt();
  
  Serial.print("handling led "); Serial.print(ledId);
  Serial.print(" state "); Serial.println(state);

  leds.state[ledId] = state; 

  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["led"] = ledId;
  root["state"] = state;
  root.printTo(*response);
  request->send(response);
}

void handleSequences(AsyncWebServerRequest * request) {
  if (request->params() != 1) return;

  int seqId = request->getParam(0)->value().toInt();
  Serial.println("handling seq " + request->getParam(0)->value());
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

  // start SPIFFS 
  Serial.print("\n\nSetting SPIFFS ... ");
  while (!SPIFFS.begin()) {
    delay(500);
  };
  Serial.println("OK");

  // set acces point
  Serial.print("Setting access point... ");
  while (!WiFi.softAP(ssid, password)) {
    delay(500);
  }
  Serial.println("OK");

  IPAddress APIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(APIP);

  // set web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led", HTTP_GET, handleLeds);
  server.on("/seq", HTTP_GET, handleSequences);
  

  // set bootstrap routes
  server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/bootstrap.bundle.min.js", "text/javascript");
  });
   
  server.on("/src/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/jquery-3.3.1.min.js", "text/javascript");
  });
   
  server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/src/bootstrap.min.css", "text/css");
  });

  server.begin();
  Serial.println("HTTP server is running\n\n\n");
}


// *************** LOOP *************************************************
void loop() {

  for(int i=0; i<N; i++){
      if (leds.state[i])
        digitalWrite(leds.nums[i], leds.on_s[i]);
      else
        digitalWrite(leds.nums[i], leds.off_s[i]);
    }

}
