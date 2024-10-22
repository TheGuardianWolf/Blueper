#pragma once

#include "Interfaces.h"
#include <ESP32Time.h>

class Timing : public ITiming
{
public:
    // Prevent creation
    Timing(Timing const &) = delete;
    void operator=(Timing const &) = delete;

    static Timing &create();

    unsigned long getTimestamp() override;

private:
    Timing() {}

    static ESP32Time s_espTime;
};
