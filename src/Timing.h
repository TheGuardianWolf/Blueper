#pragma once

#include "Interfaces.h"
#include <ESP32Time.h>
#include <mutex>
#include <list>

class Timing : public ITiming
{
public:
    // Prevent creation
    Timing(Timing const &) = delete;
    void operator=(Timing const &) = delete;

    static Timing &create();

    void loop() override;
    unsigned long long getTimestamp() override;
    void scheduleEvent(std::function<bool()>, unsigned long long period, unsigned long long startTime = 0) override;
    void scheduleEventAbsolute(std::function<bool()>, unsigned long long period, unsigned long long startTimeAbsolute = 0) override;

private:
    struct ScheduledEventOptions
    {
        size_t m_id = 0;
        std::function<bool()> m_callback = nullptr;
        unsigned long long m_period = 0;
        unsigned long long m_startTime = 0;
        unsigned long long m_scheduledTime = 0;
        bool m_startTimeIsAbsolute = false;
        unsigned long long m_lastExecutionTime = 0;
    };

    Timing() {}

    static bool s_init;
    static ESP32Time s_espTime;

    std::mutex m_lock;
    size_t m_nextID = 1;
    // Returns whether it should continue or not (bool)
    std::list<ScheduledEventOptions> m_scheduledEvents;
};
