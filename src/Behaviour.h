#pragma once

#include <memory>
#include "Interfaces.h"

class Behaviour
{
public:
    Behaviour(IAdvertisement &advertisement, IScanner &scanner, IBlinker &blinker);
    void run();

private:
    IAdvertisement &m_advertisement;
    IScanner &m_scanner;
    IBlinker &m_blinker;

    void handleScanResult(int rssi);
};
