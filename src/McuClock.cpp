#include "McuClock.h"
#include "Utils.h"

#include <Arduino.h>

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>



void McuClock::update() 
{
    timeNow = millis()/1000UL; // the number of milliseconds that have passed since boot
    seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.

    if (seconds == 60) 
    {
        timeLast = timeNow;
        minutes = minutes + 1;
    }

    //if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

    if (minutes == 60)
    { 
        minutes = 0;
        hours = hours + 1;
    }

    // if one hour has passed, start counting minutes from zero and add one hour to the clock

    if (hours == 24)
    {
        hours = 0;
        days = days + 1;
    }

    //if 24 hours have passed , add one day

    if (hours == (24 - startingHour) && correctedToday == 0)
    {
        delay(dailyErrorFast*1000);
        seconds = seconds + dailyErrorBehind;
        correctedToday = 1;
    }

    //every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
    //Change these varialbes according to the error of your board. 
    // The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
    // and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

    if (hours == 24 - startingHour + 2) 
    { 
        correctedToday = 0;
    }

//let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
// would continue to correct for an entire hour that is 24 - startingHour. 
}

void McuClock::doNtpSync() 
{
   auto ntp_time = utils::getNtpTime();
   if(ntp_time == 0) return;

//    putenv("TZ=Asia/Singapore");

   struct tm* ntp_tm = localtime(&ntp_time);
   seconds = ntp_tm->tm_sec;
   minutes = ntp_tm->tm_min;
   hours = ntp_tm->tm_hour;
   days = ntp_tm->tm_yday;
   startingHour = hours;

   correctedToday = 1;
}

void McuClock::setTime(int32_t s, int32_t m, int32_t h, int32_t d) 
{
    seconds = s;
    minutes = m;
    hours = h;
    days = d;
    startingHour = h;
}
