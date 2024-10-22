#pragma once

class IAdvertisement
{
public:
    virtual ~IAdvertisement() = default;
    virtual void start() = 0;
};

class IScanner
{
public:
    virtual ~IScanner() = default;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual int getRSSI() const = 0;
    virtual void setRSSI(const int rssi) = 0;
};

class IBlinker
{
public:
    virtual ~IBlinker() = default;
    virtual void setBlinkDelay(int delay) = 0;
    virtual void blink() = 0;
};

class ITime
{
public:
    virtual ~ITime() = default;
};
