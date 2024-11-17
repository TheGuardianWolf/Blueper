#include "Timing.h"
#include <set>
#include <algorithm>

// Singleton
bool Timing::s_init = false;
ESP32Time Timing::s_espTime;

Timing &Timing::create()
{
    static Timing instance;

    if (!s_init)
    {
        s_init = true;
        s_espTime.setTime(0);
    }

    return instance;
}

void Timing::loop()
{
    auto ts = getTimestamp();

    std::list<Timing::ScheduledEventOptions> scheduledEvents;

    {
        std::lock_guard lock{m_lock};
        scheduledEvents.insert(scheduledEvents.end(), m_scheduledEvents.begin(), m_scheduledEvents.end());
    }

    std::set<size_t> unscheduleForIDs;
    std::set<size_t> rescheduleForIDs;
    for (auto &options : scheduledEvents)
    {
        if (ts < options.m_nextExecutionTime)
        {
            continue;
        }

        Serial.printf("Running scheduled event with id %d at ts: %llu\n", options.m_id, ts);
        auto shouldUnschedule = options.m_callback() || options.m_period == 0;

        if (shouldUnschedule)
        {
            unscheduleForIDs.insert(options.m_id);
        }
        else
        {
            rescheduleForIDs.insert(options.m_id);
        }
    }

    {
        std::lock_guard lock{m_lock};
        m_scheduledEvents.remove_if(
            [&unscheduleForIDs](auto &options)
            {
                return static_cast<bool>(unscheduleForIDs.count(options.m_id));
            });
        for (auto &options : m_scheduledEvents)
        {
            if (rescheduleForIDs.find(options.m_id) != rescheduleForIDs.end())
            {
                options.m_nextExecutionTime += options.m_period;
            }
        }
    }
}

unsigned long long Timing::getTimestamp()
{
    return static_cast<unsigned long long>(s_espTime.getEpoch()) * 1000 + s_espTime.getMillis();
}

size_t Timing::scheduleEvent(std::function<bool()> callback, unsigned long long period, unsigned long long startTime)
{
    auto ts = getTimestamp();

    ScheduledEventOptions options{
        .m_id = m_nextID,
        .m_callback = callback,
        .m_period = period,
        .m_scheduledTime = ts,
        .m_nextExecutionTime = ts + startTime};

    std::lock_guard lock{m_lock};
    m_nextID += 1;
    m_scheduledEvents.push_back(options);

    return options.m_id;
}

size_t Timing::scheduleEventAbsolute(std::function<bool()> callback, unsigned long long period, unsigned long long startTimeAbsolute)
{
    auto ts = getTimestamp();

    ScheduledEventOptions options{
        .m_id = m_nextID,
        .m_callback = callback,
        .m_period = period,
        .m_scheduledTime = ts,
        .m_nextExecutionTime = startTimeAbsolute};

    std::lock_guard lock{m_lock};
    m_nextID += 1;
    m_scheduledEvents.push_back(options);

    return options.m_id;
}

bool Timing::removeEvent(size_t id)
{
    std::lock_guard lock{m_lock};

    auto optionsIt = std::find_if(m_scheduledEvents.begin(), m_scheduledEvents.end(),
                                  [&id](auto &options)
                                  { return options.m_id == id; });

    if (optionsIt == m_scheduledEvents.end())
    {
        return false;
    }

    m_scheduledEvents.erase(optionsIt);
    return true;
}
