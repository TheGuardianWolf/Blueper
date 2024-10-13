#pragma once

#include "Interfaces.h"

class Blinker : public IBlinker
{
public:
    Blinker(int pin);
    void setBlinkDelay(int delay) override;
    void blink() override;

private:
    int ledPin;
    int blinkDelay;
};
