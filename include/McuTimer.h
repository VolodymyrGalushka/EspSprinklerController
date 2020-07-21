#ifndef __MCUTIMER_H__
#define __MCUTIMER_H__

#include <Arduino.h>

#include <functional>

class McuTimer
{
    public:
        McuTimer(uint64_t interval, bool repeat = false);
        ~McuTimer() = default;

        void        addIntervalHandler(std::function<void()> handler);
        void        start();
        void        stop();
        void        update();
        uint64_t    currentSeconds() const;

    private:
        std::vector<std::function<void()>>      m_handlers;
        bool                                    m_repeat{false};
        uint64_t                                m_interval{0};
        uint64_t                                m_current{0};
        uint64_t                                m_last{0};
        bool                                    m_stop{false};

};


#endif // __MCUTIMER_H__