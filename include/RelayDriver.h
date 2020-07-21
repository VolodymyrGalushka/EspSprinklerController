#ifndef __RELAYDRIVER_H__
#define __RELAYDRIVER_H__

#include <Arduino.h>


enum class RelayState
{
    High = HIGH,
    Low = LOW
};

class RelayDriver
{

    public:
        RelayDriver(uint8_t controlPin = D1, RelayState initialState = RelayState::Low);
        ~RelayDriver() = default;

        void             toggle();
        RelayState       state() const;
        void             setState(RelayState state);

    private:
        RelayState       m_state{RelayState::Low};
        uint8_t          m_controlPin{D1};
};




#endif // __RELAYDRIVER_H__