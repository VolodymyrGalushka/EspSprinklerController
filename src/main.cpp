#include <Arduino.h>
#include "WebServer.h"
#include "McuTimer.h"
#include "McuClock.h"
#include "Utils.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <FS.h>
#else
#include <WiFi.h>
#include <SPIFFS.h>
#endif

#include <functional>
#include <memory>

#define AP_SSID "SprinklerController"
#define AP_PASSWD "87654321"


McuClock g_clock{};
std::unique_ptr<McuTimer> g_timer{nullptr};

void setup() 
{

  Serial.begin(115200);
  // Serial.print("Setting AP (Access Point)…");
  // WiFi.softAP(AP_SSID, AP_PASSWD);

  // IPAddress ap_ip = WiFi.softAPIP();
  // Serial.print("AP IP: ");
  // Serial.println(ap_ip);

  // Serial.print("Local node IP: ");
  // Serial.println(WiFi.localIP());

  //switch to WIFI STA
  WiFi.begin("IT-CROWD", "fulldome108");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  g_clock.doNtpSync();

}

void loop() 
{

  g_clock.update();

  auto timestr = g_clock.getTimeString();

  Serial.print(timestr.c_str());
  Serial.println();

  delay(1000);  
  
}