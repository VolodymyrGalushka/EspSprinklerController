#ifndef __CLOCKS_H__
#define __CLOCKS_H__

#include <Arduino.h>
#include <string>

class McuClock
{
    public:
        McuClock() = default;
        ~McuClock() = default;

        void                doNtpSync();
        void                setTime(int32_t s, int32_t m, int32_t h, int32_t d);
        void                update();
        std::string         getTimeString() const;

    private:
        uint64_t            timeNow{0};
        uint64_t            timeLast{0};
        int32_t             startingHour{0}; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
        int32_t             seconds{0};
        int32_t             minutes{0};
        int32_t             hours{0};
        int32_t             days{0};


        int32_t             dailyErrorFast{0}; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
        int32_t             dailyErrorBehind{0}; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
        int32_t             correctedToday{0}; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  
};


#endif // __CLOCKS_H__