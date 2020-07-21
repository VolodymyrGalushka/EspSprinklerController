#include <Arduino.h>


#include <WiFiUdp.h>

extern WiFiUDP g_udp;

namespace utils
{
    time_t getNtpTime();
}
