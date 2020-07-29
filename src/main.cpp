#include <Arduino.h>
#include "main.h"
#include "WebServer.h"
#include "McuClock.h"
#include "McuConfig.h"
#include "RelayDriver.h"
#include "MoistureSensorDriver.h"
#include "RainSensorDriver.h"
#include "Utils.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <FS.h>
#else
#include <WiFi.h>
#include <SPIFFS.h>
#endif

#include <Ticker.h>
#include <CircularBuffer.h>

#include <functional>
#include <memory>

#define AP_SSID "SprinklerController"
#define AP_PASSWD "87654321"

McuClock g_clock{};
CircularBuffer<std::string, 50> g_log_buf;
std::map<int, std::unique_ptr<RelayDriver>> g_valves;
std::unique_ptr<McuConfig> g_mcu_config{nullptr};
std::unique_ptr<MoistureSensorDriver> g_moist_sensor{nullptr};
std::unique_ptr<RainSensorDriver> g_rain_sensor{nullptr};


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

void toggleValve(uint8_t number)
{
  g_valves[number]->toggle();
}

void loop() 
{

  g_clock.update();

  auto timestr = g_clock.getTimeString();

  Serial.print(timestr.c_str());
  Serial.println();


  delay(5000);
  
}


void configure() 
{
    g_mcu_config.reset(new McuConfig());
    SPIFFS.begin();
    auto exists = SPIFFS.exists("/mcu_config.json");
    SPIFFS.end();
    if(!exists)
    {
        g_log_buf.unshift("Mcu config file doesn't exist!");
        return;
    }
    else
    {
        g_mcu_config->deserializeFromFile("/mcu_config.json");
    }

    //create valve drivers
    for(const auto& valve : g_mcu_config->valves())
    {
        auto valve_it = g_valves.find(valve.first);
        if(valve_it != g_valves.end())
        {
            //update
        }
        else
        {
          g_valves[valve.first] = std::move(std::unique_ptr<RelayDriver>(new RelayDriver(valve.second.pin)));
        }
    }

    if(g_mcu_config->rainSensorEnabled)
    {
        g_rain_sensor.reset(new RainSensorDriver(g_mcu_config->rainSensorPin));
    }

    if(g_mcu_config->moistureSensorEnabled)
    {
        g_moist_sensor.reset(new MoistureSensorDriver(g_mcu_config->moistureSensorPin));
        g_moist_sensor->set0(g_mcu_config->moistLow);
        g_moist_sensor->set100(g_mcu_config->moistHigh);
    }

}
