#ifndef __RAINSENSORDRIVER_H__
#define __RAINSENSORDRIVER_H__

#include <Arduino.h>

enum class  RainStrength
{
    Flood = 2,
    Wet = 1,
    NotRaining = 0
};

class RainSensorDriver
{

    public:
        RainSensorDriver(std::uint8_t dataPin);
        ~RainSensorDriver() = default;

        int32_t             getValue();
        RainStrength        getRainStrength();

    private:
        uint8_t             m_dataPin{A0};
        int32_t             m_lastValue{0};

};


#endif // __RAINSENSORDRIVER_H__