#include "Behaviour.h"
#include <Arduino.h>

Behaviour::Behaviour(std::shared_ptr<IAdvertisement> pAdvertisement, std::shared_ptr<IScanner> pScanner, std::shared_ptr<IBlinker> pBlinker)
    : m_pAdvertisement(pAdvertisement), m_pScanner(pScanner), m_pBlinker(pBlinker) {}

void Behaviour::run()
{
    m_pAdvertisement->start();
    m_pScanner->start();

    while (true)
    {
        m_pScanner->update();
        int rssi = m_pScanner->getLatestRSSI();
        handleScanResult(rssi);
    }
}

void Behaviour::handleScanResult(int rssi)
{
    if (rssi != 0)
    {                                                   // Assuming 0 means no valid RSSI
        int blinkDelay = map(rssi, -100, 0, 1000, 100); // Adjust the range as needed
        m_pBlinker->setBlinkDelay(blinkDelay);
        m_pBlinker->blink();
    }
}
