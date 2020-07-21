#include "RainSensorDriver.h"

const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

RainSensorDriver::RainSensorDriver(std::uint8_t dataPin) : m_dataPin{dataPin}
{
    
}


int32_t RainSensorDriver::getValue() 
{
    int32_t value = analogRead(m_dataPin);
    m_lastValue = value;
    return value;
}

RainStrength RainSensorDriver::getRainStrength() 
{
    int32_t value = analogRead(m_dataPin);
    int range = map(value, sensorMin, sensorMax, 0, 3);
    return static_cast<RainStrength>(range);
}

