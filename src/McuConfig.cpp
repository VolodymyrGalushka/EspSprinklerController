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
    m_valves[valve.name] = valve;    
}

Valve McuConfig::valve(std::string name) const
{
    return m_valves.at(name);
}

void McuConfig::updateZone(const Zone& zone) 
{
    m_zones[zone.name] = zone;
}

Zone McuConfig::zone(std::string name) const
{
    return m_zones.at(name);
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
    for (auto v = m_valves.begin(); v != m_valves.end(); ++v) 
    {
        auto valve_obj = valves_array.createNestedObject();
        valve_obj["name"] = v->second.name;
        valve_obj["pin"] = v->second.pin;
        valve_obj["enabled"] = v->second.enabled;
        valve_obj["toggleDelay"] = v->second.toggleDelay;
        // valves_array.add(valve_obj);
    }
    auto zones_array = root.createNestedArray("zones");
    for (auto z = m_zones.begin(); z != m_zones.end(); ++z) 
    {
        auto zone_obj = zones_array.createNestedObject();
        zone_obj["name"] = z->second.name;

        auto trigger_array = zone_obj.createNestedArray("triggers");
        for(auto trig = z->second.triggers.begin(); trig != z->second.triggers.end(); ++trig)
        {
            auto trig_obj = trigger_array.createNestedObject();
            trig_obj["minute"] = trig->minute;
            trig_obj["hour"] = trig->hour;
            
            auto wd = trig_obj.createNestedArray("daysOfWeek");
            for(auto d : trig->daysOfWeek)
            {
                wd.add(d);
            }

            auto md = trig_obj.createNestedArray("daysOfMonth");
            for(auto d : trig->daysOfMonth)
            {
                md.add(d);
            }

            trig_obj["once"] = trig->once; 
            trig_obj["sprinkleDuration"] = trig->sprinkleDuration;
        }

        auto zone_valves_array = zone_obj.createNestedArray("valves");
        for(auto vn : z->second.valves)
        {
            zone_valves_array.add(vn);
        }
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
    for (auto v = m_valves.begin(); v != m_valves.end(); ++v) 
    {
        auto valve_obj = valves_array.createNestedObject();
        valve_obj["name"] = v->second.name;
        valve_obj["pin"] = v->second.pin;
        valve_obj["enabled"] = v->second.enabled;
        valve_obj["toggleDelay"] = v->second.toggleDelay;
        // valves_array.add(valve_obj);
    }
    auto zones_array = root.createNestedArray("zones");
    for (auto z = m_zones.begin(); z != m_zones.end(); ++z) 
    {
        auto zone_obj = zones_array.createNestedObject();
        zone_obj["name"] = z->second.name;

        auto trigger_array = zone_obj.createNestedArray("triggers");
        for(auto trig = z->second.triggers.begin(); trig != z->second.triggers.end(); ++trig)
        {
            auto trig_obj = trigger_array.createNestedObject();
            trig_obj["minute"] = trig->minute;
            trig_obj["hour"] = trig->hour;
            
            auto wd = trig_obj.createNestedArray("daysOfWeek");
            for(auto d : trig->daysOfWeek)
            {
                wd.add(d);
            }

            auto md = trig_obj.createNestedArray("daysOfMonth");
            for(auto d : trig->daysOfMonth)
            {
                md.add(d);
            }

            trig_obj["once"] = trig->once; 
            trig_obj["sprinkleDuration"] = trig->sprinkleDuration;
        }

        auto zone_valves_array = zone_obj.createNestedArray("valves");
        for(auto vn : z->second.valves)
        {
            zone_valves_array.add(vn);
        }
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

    m_zones.clear();
    m_valves.clear();

    rainDelayOn = doc["rainDelayOn"];
    moistLow = doc["moistLow"];
    moistHigh = doc["moistHigh"];
    moistThreshold = doc["moistThreshold"];
    considerMoisture = doc["considerMoisture"];

    for(JsonVariant vv : doc["valves"].as<JsonArray>())
    {
        auto valve_obj = vv.as<JsonObject>();
        Valve valve{};
        valve.name = valve_obj["name"].as<std::string>();
        valve.pin = valve_obj["pin"].as<uint8_t>();
        valve.enabled = valve_obj["enabled"].as<bool>();
        valve.toggleDelay = valve_obj["toggleDelay"].as<uint32_t>();
        m_valves[valve.name] = valve;
    }

    for(JsonVariant z : doc["zones"].as<JsonArray>())
    {
        auto zone_obj = z.as<JsonObject>();
        Zone zone{};
        zone.name = zone_obj["name"].as<std::string>();

        for(JsonVariant t : zone_obj["triggers"].as<JsonArray>())
        {
            auto trigger_obj = t.as<JsonObject>();
            Trigger trigger{};
            trigger.once = trigger_obj["once"].as<bool>();
            trigger.minute = trigger_obj["minute"].as<int32_t>();
            trigger.hour = trigger_obj["hour"].as<int32_t>();
            trigger.sprinkleDuration = trigger_obj["sprinkleDuration"].as<uint64_t>();
            auto weekdays = trigger_obj["daysOfWeek"].as<JsonArray>();
            for(auto wd : weekdays)
            {
                trigger.daysOfWeek.push_back(wd);
            }
            auto monthdays = trigger_obj["daysOfMonth"].as<JsonArray>();
            for(auto md : monthdays)
            {
                trigger.daysOfWeek.push_back(md);
            }
            zone.triggers.push_back(trigger);
        }

        for(JsonVariant v : zone_obj["valves"].as<JsonArray>())
        {
            zone.valves.push_back(v.as<std::string>());
        }

        m_zones[zone.name] = zone;
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

    m_zones.clear();
    m_valves.clear();

    rainDelayOn = doc["rainDelayOn"];
    moistLow = doc["moistLow"];
    moistHigh = doc["moistHigh"];
    moistThreshold = doc["moistThreshold"];
    considerMoisture = doc["considerMoisture"];

    for(JsonVariant vv : doc["valves"].as<JsonArray>())
    {
        auto valve_obj = vv.as<JsonObject>();
        Valve valve{};
        valve.name = valve_obj["name"].as<std::string>();
        valve.pin = valve_obj["pin"].as<uint8_t>();
        valve.enabled = valve_obj["enabled"].as<bool>();
        valve.toggleDelay = valve_obj["toggleDelay"].as<uint32_t>();
        m_valves[valve.name] = valve;
    }

    for(JsonVariant z : doc["zones"].as<JsonArray>())
    {
        auto zone_obj = z.as<JsonObject>();
        Zone zone{};
        zone.name = zone_obj["name"].as<std::string>();

        for(JsonVariant t : zone_obj["triggers"].as<JsonArray>())
        {
            auto trigger_obj = t.as<JsonObject>();
            Trigger trigger{};
            trigger.once = trigger_obj["once"].as<bool>();
            trigger.minute = trigger_obj["minute"].as<int32_t>();
            trigger.hour = trigger_obj["hour"].as<int32_t>();
            trigger.sprinkleDuration = trigger_obj["sprinkleDuration"].as<uint64_t>();
            auto weekdays = trigger_obj["daysOfWeek"].as<JsonArray>();
            for(auto wd : weekdays)
            {
                trigger.daysOfWeek.push_back(wd);
            }
            auto monthdays = trigger_obj["daysOfMonth"].as<JsonArray>();
            for(auto md : monthdays)
            {
                trigger.daysOfWeek.push_back(md);
            }
            zone.triggers.push_back(trigger);
        }

        for(JsonVariant v : zone_obj["valves"].as<JsonArray>())
        {
            zone.valves.push_back(v.as<std::string>());
        }

        m_zones[zone.name] = zone;
    }

    cfg_file.close();
    SPIFFS.end();
}
