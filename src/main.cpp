#include <Arduino.h>
#include "WebServer.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <FS.h>
#else
#include <WiFi.h>
#include <SPIFFS.h>
#endif

#define AP_SSID "SprinklerController"
#define AP_PASSWD "87654321"

void setup() 
{

  Serial.begin(115200);
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(AP_SSID, AP_PASSWD);

  IPAddress ap_ip = WiFi.softAPIP();
  Serial.print("AP IP: ");
  Serial.println(ap_ip);

  Serial.print("Local node IP: ");
  Serial.println(WiFi.localIP());

}

void loop() 
{
  // put your main code here, to run repeatedly:
}