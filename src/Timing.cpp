#include "Timing.h"

ESP32Time Timing::s_espTime(0);

// Singleton
Timing &Timing::create()
{
    static Timing instance;
    instance.s_espTime.setTime(0);

    return instance;
}

unsigned long Timing::getTimestamp()
{
    return s_espTime.getEpoch();
}
