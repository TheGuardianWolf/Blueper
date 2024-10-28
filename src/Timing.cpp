#include "Timing.h"
#include <set>

// Singleton
Timing &Timing::create()
{
    static Timing instance;

    std::lock_guard lock{instance.m_lock};
    if (!s_init)
    {
        s_init = true;
        instance.s_espTime.setTime(0);
    }

    return instance;
}

void Timing::loop()
{
    auto ts = getTimestamp();

    std::lock_guard lock{m_lock};

    std::set<size_t> unscheduleForIDs;
    for (auto &options : m_scheduledEvents)
    {
        auto shouldStart = options.m_startTimeIsAbsolute && ts >= options.m_startTime ||
                           !options.m_startTimeIsAbsolute && (ts - options.m_scheduledTime) >= (options.m_startTime - options.m_scheduledTime);
        auto periodElapsed = options.m_period == 0 || options.m_lastExecutionTime == 0 || (ts - options.m_lastExecutionTime) >= options.m_period;
        if (!shouldStart || !periodElapsed)
        {
            continue;
        }

        auto shouldUnschedule = options.m_callback();
        if (shouldUnschedule)
        {
            unscheduleForIDs.insert(options.m_id);
        }
    }

    m_scheduledEvents.remove_if(
        [&unscheduleForIDs](auto &el)
        {
            auto &[options, _] = el;
            return static_cast<bool>(unscheduleForIDs.count(options.m_id));
        });
}

unsigned long long Timing::getTimestamp()
{
    return static_cast<unsigned long long>(s_espTime.getEpoch()) * 1000 + s_espTime.getMillis();
}

void Timing::scheduleEvent(std::function<bool()> callback, unsigned long long period, unsigned long long startTime = 0)
{
    auto ts = getTimestamp();

    ScheduledEventOptions options{
        .m_id = m_nextID,
        .m_callback = callback,
        .m_period = period,
        .m_startTime = startTime,
        .m_scheduledTime = ts,
        .m_startTimeIsAbsolute = false,
        .m_lastExecutionTime = 0};

    std::lock_guard lock{m_lock};
    m_nextID += 1;
    m_scheduledEvents.push_back(options);
}

void Timing::scheduleEventAbsolute(std::function<bool()> callback, unsigned long long period, unsigned long long startTimeAbsolute = 0)
{
    auto ts = getTimestamp();

    ScheduledEventOptions options{
        .m_id = m_nextID,
        .m_callback = callback,
        .m_period = period,
        .m_startTime = startTimeAbsolute,
        .m_scheduledTime = ts,
        .m_startTimeIsAbsolute = true,
        .m_lastExecutionTime = 0};

    std::lock_guard lock{m_lock};
    m_nextID += 1;
    m_scheduledEvents.push_back(options);
}
