#include "Blinker.h"
#include <Arduino.h>

Blinker::Blinker(ITiming &timing, int pin)
    : m_timing(timing), m_ledPin(pin), m_blinkDelay(OFF)
{
    pinMode(m_ledPin, OUTPUT);
}

void Blinker::setBlinkDelay(int delay)
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
    else if (delay < 0)
    {
        digitalWrite(m_ledPin, LOW);
    }
    else
    {
        auto period = static_cast<unsigned long long>(delay);
        auto id1 = m_timing.scheduleEvent(
            [this]()
            {
                digitalWrite(m_ledPin, LOW);
                return false;
            },
            period, 0);
        auto id2 = m_timing.scheduleEvent(
            [this]()
            {
                digitalWrite(m_ledPin, HIGH);
                return false;
            },
            period, period - period / 4);

        m_timingIDs.insert({id1, id2});
    }
}
