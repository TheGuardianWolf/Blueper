#include "Blinker.h"
#include <Arduino.h>

Blinker::Blinker(int pin) : ledPin(pin), blinkDelay(1000)
{
    pinMode(ledPin, OUTPUT);
}

void Blinker::setBlinkDelay(int delay)
{
    blinkDelay = delay;
}

void Blinker::blink()
{
    digitalWrite(ledPin, HIGH);
    delay(blinkDelay);
    digitalWrite(ledPin, LOW);
    delay(blinkDelay);
}
