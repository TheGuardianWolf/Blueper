#pragma once

#include <memory>
#include "Interfaces.h"

class Behaviour
{
public:
    Behaviour(ITiming &timing, IAdvertisement &advertisement, IScanner &scanner, IBlinker &blinker);
    void start();

private:
    ITiming &m_timing;
    IAdvertisement &m_advertisement;
    IScanner &m_scanner;
    IBlinker &m_blinker;

    void handleScanResult(int rssi);

    size_t m_blinkRefreshID = 0;
    int m_currentPathLoss = 0;
};
