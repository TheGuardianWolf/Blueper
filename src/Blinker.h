#pragma once

#include "Interfaces.h"
#include <set>

class Blinker : public IBlinker
{
public:
    static const int INFINITE = -2;
    static const int OFF = -1;

    Blinker(ITiming &timing, int pin);
    void setBlinkDelay(int delay) override;

private:
    ITiming &m_timing;
    int m_ledPin;
    int m_currentBlinkDelay;
    int m_nextBlinkDelay;
    std::set<size_t> m_timingIDs;

    void setBlinkDelayInternal(int delay);
};
