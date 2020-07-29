#ifndef __MCUCONFIG_H__
#define __MCUCONFIG_H__

#include <string>
#include <map>

struct Valve
{
    uint8_t     pin;
    bool        enabled{false};
    uint32_t    toggleDelay{}; // in seconds
    uint64_t    openDuration{}; // in seconds
};


class McuConfig
{

    public:
        McuConfig() = default;
        ~McuConfig() = default;

        std::string                         serialize() const;
        void                                serializeToFile(const std::string& cfg_filename);
        void                                deserialize(const std::string& json);
        void                                deserializeFromFile(const std::string& cfg_filename);

        void                                updateValve(const Valve& valve);
        Valve                               valve(int pin) const;
        const std::map<int, Valve>&         valves(){ return m_valves; };

        bool                                rainSensorEnabled{false};
        bool                                moistureSensorEnabled{true};

        uint8_t                             rainSensorPin{A0};
        uint8_t                             moistureSensorPin{A0};

        bool                                rainDelayOn{true};
        int32_t                             moistLow{};
        int32_t                             moistHigh{};
        int32_t                             moistThreshold{};
        bool                                considerMoisture{false};

    private:
        std::map<int, Valve>                m_valves;
};




#endif // __MCUCONFIG_H__