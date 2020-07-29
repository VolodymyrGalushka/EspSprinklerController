#include "McuConfig.h"

#include <ArduinoJson.h>
#ifdef ESP8266
#include <FS.h>
#else
#include <SPIFFS.h>
#endif

#include <CircularBuffer.h>

#include <sstream>

extern CircularBuffer<std::string, 50> g_log_buf;

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
    JsonObject root = doc.as<JsonObject>();
    root["rainDelayOn"] = rainDelayOn;
    root["moistLow"] = moistLow;
    root["moistHigh"] = moistHigh;
    root["moistThreshold"] = moistThreshold;
    root["considerMoisture"] = considerMoisture;
    auto valves_array = root.createNestedArray("valves");
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
    JsonObject root = doc.as<JsonObject>();
    root["rainDelayOn"] = rainDelayOn;
    root["moistLow"] = moistLow;
    root["moistHigh"] = moistHigh;
    root["moistThreshold"] = moistThreshold;
    root["considerMoisture"] = considerMoisture;
    auto valves_array = root.createNestedArray("valves");
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
            g_log_buf.unshift("McuConfig::deserialize -- Deserialization succeeded");
            return;
        case DeserializationError::InvalidInput:
            g_log_buf.unshift("McuConfig::deserialize -- Invalid input!");
            return;
        case DeserializationError::NoMemory:
            g_log_buf.unshift("McuConfig::deserialize -- Not enough memory");
            return;
        default:
            g_log_buf.unshift("McuConfig::deserialize -- Deserialization failed");
            return;
    }

    rainDelayOn = doc["rainDelayOn"];
    moistLow = doc["moistLow"];
    moistHigh = doc["moistHigh"];
    moistThreshold = doc["moistThreshold"];
    considerMoisture = doc["considerMoisture"];

    for(JsonVariant vv : doc["valves"].as<JsonArray>())
    {
        auto valve_obj = vv.as<JsonObject>();
        Valve valve{};
        valve.pin = valve_obj["pin"].as<uint8_t>();
        valve.enabled = valve_obj["enabled"].as<bool>();
        valve.toggleDelay = valve_obj["toggleDelay"].as<uint32_t>();
        valve.openDuration = valve_obj["openDuration"].as<uint64_t>();
        m_valves[valve.pin] = valve;
    }

}

void McuConfig::deserializeFromFile(const std::string& cfg_filename) 
{
    SPIFFS.begin();
    File cfg_file = SPIFFS.open(cfg_filename.c_str(), "r");
    DynamicJsonDocument doc(1024);
    auto err = deserializeJson(doc, cfg_file);
    switch (err.code()) 
    {
        case DeserializationError::Ok:
            g_log_buf.unshift("McuConfig::deserialize -- Deserialization succeeded");
            return;
        case DeserializationError::InvalidInput:
            g_log_buf.unshift("McuConfig::deserialize -- Invalid input!");
            return;
        case DeserializationError::NoMemory:
            g_log_buf.unshift("McuConfig::deserialize -- Not enough memory");
            return;
        default:
            g_log_buf.unshift("McuConfig::deserialize -- Deserialization failed");
            return;
    }

    rainDelayOn = doc["rainDelayOn"];
    moistLow = doc["moistLow"];
    moistHigh = doc["moistHigh"];
    moistThreshold = doc["moistThreshold"];
    considerMoisture = doc["considerMoisture"];

    for(JsonVariant vv : doc["valves"].as<JsonArray>())
    {
        auto valve_obj = vv.as<JsonObject>();
        Valve valve{};
        valve.pin = valve_obj["pin"].as<uint8_t>();
        valve.enabled = valve_obj["enabled"].as<bool>();
        valve.toggleDelay = valve_obj["toggleDelay"].as<uint32_t>();
        valve.openDuration = valve_obj["openDuration"].as<uint64_t>();
        m_valves[valve.pin] = valve;
    }
    cfg_file.close();
    SPIFFS.end();
}
