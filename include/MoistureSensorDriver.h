#ifndef __MOISTURESENSORDRIVER_H__
#define __MOISTURESENSORDRIVER_H__



#include <Arduino.h>

class MoistureSensorDriver
{

    public:
        MoistureSensorDriver(uint8_t dataPin);
        ~MoistureSensorDriver() = default;

        void        calibrate0();
        void        calibrate100();

        int32_t     getValue();
        int32_t     getPercents();

    private:
        uint8_t     m_dataPin{A0};
        int32_t     m_lastValue;
        int32_t     m_low{};
        int32_t     m_high{};
};




#endif // __MOISTURESENSORDRIVER_H__