#include "McuConfig.h"

#include <ArduinoJson.h>
#ifdef ESP8266
#include <FS.h>
#else
#include <SPIFFS.h>
#endif

#include <sstream>

void McuConfig::updateValve(const Valve& valve) 
{
    m_valves[valve.pin] = valve;    
}

Valve McuConfig::valve(int pin) const
{
    return m_valves.at(pin);
}

std::string McuConfig::serialize() const
{
    DynamicJsonDocument doc(1024);
    JsonObject obj = doc.to<JsonObject>();
    obj["rainDelayOn"] = rainDelayOn;
    obj["moistLow"] = moistLow;
    obj["moistHigh"] = moistHigh;
    obj["moistThreshold"] = moistThreshold;
    obj["considerMoisture"] = considerMoisture;
    auto valves_array = doc.createNestedArray("valves");
    for (auto it = m_valves.begin(); it != m_valves.end(); ++it) 
    {
        auto valve_obj = valves_array.createNestedObject();
        valve_obj["pin"] = it->second.pin;
        valve_obj["enabled"] = it->second.enabled;
        valve_obj["toggleDelay"] = it->second.toggleDelay;
        valve_obj["openDuration"] = it->second.openDuration;
        valves_array.add(valve_obj);
    }
    std::stringstream jsonstream;
    serializeJson(doc, jsonstream);
    return jsonstream.str();
}

void McuConfig::serializeToFile(const std::string& cfg_filename) 
{
    SPIFFS.begin();
    File cfg_file = SPIFFS.open(cfg_filename.c_str(), "w");
    DynamicJsonDocument doc(1024);
    JsonObject obj = doc.to<JsonObject>();
    obj["rainDelayOn"] = rainDelayOn;
    obj["moistLow"] = moistLow;
    obj["moistHigh"] = moistHigh;
    obj["moistThreshold"] = moistThreshold;
    obj["considerMoisture"] = considerMoisture;
    auto valves_array = doc.createNestedArray("valves");
    for (auto it = m_valves.begin(); it != m_valves.end(); ++it) 
    {
        auto valve_obj = valves_array.createNestedObject();
        valve_obj["pin"] = it->second.pin;
        valve_obj["enabled"] = it->second.enabled;
        valve_obj["toggleDelay"] = it->second.toggleDelay;
        valve_obj["openDuration"] = it->second.openDuration;
        valves_array.add(valve_obj);
    }

    serializeJson(doc, cfg_file);
    cfg_file.close();
    SPIFFS.end();
}

void McuConfig::deserialize(const std::string& json) 
{
    DynamicJsonDocument doc(1024);
    auto err = deserializeJson(doc, json);
    switch (err.code()) 
    {
        case DeserializationError::Ok:
            Serial.print(F("Deserialization succeeded"));
            return;
        case DeserializationError::InvalidInput:
            Serial.print(F("Invalid input!"));
            return;
        case DeserializationError::NoMemory:
            Serial.print(F("Not enough memory"));
            return;
        default:
            Serial.print(F("Deserialization failed"));
            return;
    }

    rainDelayOn = doc["rainDelayOn"];
    moistLow = doc["moistLow"];
    moistHigh = doc["moistHigh"];
    moistThreshold = doc["moistThreshold"];
    considerMoisture = doc["considerMoisture"];

    

}
