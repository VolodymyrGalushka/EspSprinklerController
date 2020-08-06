#ifndef __MCUCONFIG_H__
#define __MCUCONFIG_H__

#include <string>
#include <map>
#include <vector>

struct Valve
{
    std::string     name;
    uint8_t         pin;
    bool            enabled{false};
    uint32_t        toggleDelay{0}; // in seconds, time to wait before toggle  
};

struct Trigger
{
    int32_t                 minute{0};
    int32_t                 hour{0};
    bool                    once{false};
    std::vector<uint8_t>    daysOfWeek{};
    std::vector<uint8_t>    daysOfMonth{};
    uint64_t                sprinkleDuration{0}; // in seconds, time to hold state
};

struct Zone
{
    std::string                 name;  
    std::vector<std::string>    valves;
    std::vector<Trigger>        triggers;
};

class McuConfig
{

    public:
        McuConfig() = default;
        ~McuConfig() = default;

        std::string                             serialize() const;
        void                                    serializeToFile(const std::string& cfg_filename);
        void                                    deserialize(const std::string& json);
        void                                    deserializeFromFile(const std::string& cfg_filename);

        void                                    updateValve(const Valve& valve);
        Valve                                   valve(std::string name) const;
        const std::map<std::string, Valve>&     valves(){ return m_valves; };

        void                                    updateZone(const Zone& zone);
        Zone                                    zone(std::string name) const;
        const std::map<std::string, Zone>&      zones() { return m_zones; };

        bool                                    rainSensorEnabled{false};
        bool                                    moistureSensorEnabled{true};

        uint8_t                                 rainSensorPin{A0};
        uint8_t                                 moistureSensorPin{A0};

        bool                                    rainDelayOn{true};
        int32_t                                 moistLow{};
        int32_t                                 moistHigh{};
        int32_t                                 moistThreshold{};
        bool                                    considerMoisture{false};

    private:
        std::map<std::string, Valve>            m_valves;
        std::map<std::string, Zone>             m_zones;
};




#endif // __MCUCONFIG_H__