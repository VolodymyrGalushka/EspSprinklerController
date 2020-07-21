#include "RelayDriver.h"


RelayDriver::RelayDriver(uint8_t controlPin, RelayState initialState) : 
    m_controlPin{controlPin},
    m_state{initialState}
{
    
}

void RelayDriver::toggle() 
{
    auto val = (m_state == RelayState::Low) ? HIGH : LOW;
    digitalWrite(m_controlPin, val);
    m_state = static_cast<RelayState>(val);
}

RelayState RelayDriver::state() const
{
    return m_state;
}

void RelayDriver::setState(RelayState state) 
{
    digitalWrite(m_controlPin, static_cast<uint8_t>(state));
    m_state = state;
}
