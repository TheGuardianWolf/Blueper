#include "Blinker.h"
#include <Arduino.h>

Blinker::Blinker(ITiming &timing, int pin)
    : m_timing(timing), m_ledPin(pin), m_currentBlinkDelay(OFF), m_nextBlinkDelay(OFF)
{
    pinMode(m_ledPin, OUTPUT);
}

void Blinker::setBlinkDelay(int delay)
{
    m_nextBlinkDelay = delay;

    if (m_timingIDs.size() > 0)
    {
        return;
    }

    if (m_currentBlinkDelay != m_nextBlinkDelay)
    {
        setBlinkDelayInternal(delay);
    }
}

void Blinker::setBlinkDelayInternal(int delay)
{
    // Remove existing
    for (auto t : m_timingIDs)
    {
        m_timing.removeEvent(t);
    }
    m_timingIDs.clear();

    if (delay == INFINITE)
    {
        digitalWrite(m_ledPin, HIGH);
    }
    else if (delay == OFF || delay <= 0)
    {
        digitalWrite(m_ledPin, LOW);
    }
    else
    {
        auto period = static_cast<unsigned long long>(delay);
        auto id1 = m_timing.scheduleEvent(
            [this]()
            {
                if (m_currentBlinkDelay == m_nextBlinkDelay)
                {
                    digitalWrite(m_ledPin, HIGH);
                }
                else
                {
                    m_currentBlinkDelay = m_nextBlinkDelay;
                    setBlinkDelayInternal(m_nextBlinkDelay);
                }
                return false;
            },
            period, 0);
        auto id2 = m_timing.scheduleEvent(
            [this]()
            {
                digitalWrite(m_ledPin, LOW);
                return false;
            },
            period, period / 4);

        m_timingIDs.insert({id1, id2});
    }
}
