#include "Interfaces.h"
#include <gmock/gmock.h>

class MockArduino
{
public:
    MOCK_METHOD(void, pinMode, (int pin, int mode));
    MOCK_METHOD(void, digitalWrite, (int pin, int value));
    MOCK_METHOD(void, delay, (int ms));
};

MockArduino *mockArduino;

void pinMode(int pin, int mode)
{
    mockArduino->pinMode(pin, mode);
}

void digitalWrite(int pin, int value)
{
    mockArduino->digitalWrite(pin, value);
}

void delay(int ms)
{
    mockArduino->delay(ms);
}

class MockScanner : public IScanner
{
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(int, getLatestRSSI, (), (const, override));
};

class MockBlinker : public IBlinker
{
public:
    MOCK_METHOD(void, setBlinkDelay, (int delay), (override));
    MOCK_METHOD(void, blink, (), (override));
};
