#pragma once

#include <memory>
#include "Interfaces.h"

class Behaviour
{
public:
    Behaviour(std::shared_ptr<IAdvertisement> pAdvertisement, std::shared_ptr<IScanner> pScanner, std::shared_ptr<IBlinker> pBlinker);
    void run();

private:
    std::shared_ptr<IAdvertisement> m_pAdvertisement;
    std::shared_ptr<IScanner> m_pScanner;
    std::shared_ptr<IBlinker> m_pBlinker;

    void handleScanResult(int rssi);
};
