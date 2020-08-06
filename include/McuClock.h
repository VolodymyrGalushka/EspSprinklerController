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
        void                setTime(int s, int m, int h, int d);
        void                update();
        std::string         getTimeString() const;

        int                 getDayOfWeek() const;
        int                 getDayOfMonth() const;

    private:
        unsigned long           timeNow{0};
        unsigned long           timeLast{0};
        int                     startingHour{0}; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
        int                     seconds{0};
        int                     minutes{0};
        int                     hours{0};
        int                     days{0};


        int                 dailyErrorFast{0}; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
        int                 dailyErrorBehind{0}; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
        int                 correctedToday{0}; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  
};


#endif // __CLOCKS_H__