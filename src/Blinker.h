#pragma once

#include "Interfaces.h"
#include <set>

class Blinker : public IBlinker
{
public:
    const int INFINITE = -2;
    const int OFF = -1;

    Blinker(ITiming &timing, int pin);
    void setBlinkDelay(int delay) override;

private:
    ITiming &m_timing;
    int m_ledPin;
    int m_blinkDelay;
    std::set<size_t> m_timingIDs;
};
