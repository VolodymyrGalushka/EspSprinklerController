#include "MoistureSensorDriver.h"

MoistureSensorDriver::MoistureSensorDriver(uint8_t dataPin) : m_dataPin{dataPin}
{
    pinMode(m_dataPin, INPUT);
}


void MoistureSensorDriver::calibrate0() 
{
    m_low = analogRead(m_dataPin);
}

void MoistureSensorDriver::set0(int32_t value) 
{
    m_low = value;
}

void MoistureSensorDriver::calibrate100() 
{
    m_high = analogRead(m_dataPin);
}

void MoistureSensorDriver::set100(int32_t value) 
{
    m_high = value;
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


