#include "McuTimer.h"

McuTimer::McuTimer(uint64_t interval, bool repeat) : m_interval{interval}, m_repeat{repeat} 
{
    
}

void McuTimer::addIntervalHandler(std::function<void()> handler) 
{
    m_handlers.push_back(handler);   
}


void McuTimer::start() 
{
    m_last = millis() / 1000UL;
    m_current = 0;
    m_stop = false;
}

void McuTimer::stop() 
{
    m_stop = true;
}

void McuTimer::update() 
{
    if(m_stop) return;

    uint64_t now = millis() / 1000UL;

    if(now < m_last) // uint64 overflow happened
    {
        uint64_t elapsed = UINT64_MAX - m_last + now;
        m_current += elapsed;
    }
    else
    {
        uint64_t elapsed = now - m_last;
        m_current += elapsed;
    }
    m_last = now;

    if(m_current >= m_interval)
    {
        for(auto& handler : m_handlers)
        {
            handler();
        }
        m_current = 0;
    }
    

    if(!m_repeat) m_stop = true;

}

uint64_t McuTimer::currentSeconds() const
{
    return m_current;
}





