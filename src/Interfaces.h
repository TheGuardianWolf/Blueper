#pragma once

class IScanner;
class IBlinker;
class ITiming;
class IAdvertisement;

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

class ITiming
{
public:
    virtual ~ITiming() = default;
    virtual void loop() = 0;
    virtual unsigned long long getTimestamp() = 0;
    virtual void scheduleEvent(std::function<bool()> callback, unsigned long long period, unsigned long long startTime) = 0;
    virtual void scheduleEventAbsolute(std::function<bool()> callback, unsigned long long period, unsigned long long startTimeAbsolute) = 0;
};

class IAdvertisement
{
public:
    virtual ~IAdvertisement() = default;
    virtual void start(ITiming *pTiming) = 0;
    virtual void loop() = 0;
};
