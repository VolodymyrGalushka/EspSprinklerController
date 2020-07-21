#include "MoistureSensorDriver.h"

MoistureSensorDriver::MoistureSensorDriver(uint8_t dataPin) : m_dataPin{dataPin}
{
    
}


void MoistureSensorDriver::calibrate0() 
{
    m_low = analogRead(m_dataPin);
}

void MoistureSensorDriver::calibrate100() 
{
    m_high = analogRead(m_dataPin);
}

int32_t MoistureSensorDriver::getValue() 
{
    return analogRead(m_dataPin);
}

int32_t MoistureSensorDriver::getPercents() 
{
    auto value = analogRead(m_dataPin);
    auto percents = map(value, m_low, m_high, 0, 100);
    return percents;
}


