#include "Behaviour.h"
#include <Arduino.h>

Behaviour::Behaviour(IAdvertisement &advertisement, IScanner &scanner, IBlinker &blinker)
    : m_advertisement(advertisement), m_scanner(scanner), m_blinker(blinker) {}

void Behaviour::run()
{
    // m_advertisement.start();
    m_scanner.start();

    while (true)
    {
        m_scanner.update();
        int rssi = m_scanner.getRSSI();
        handleScanResult(rssi);
    }
}

void Behaviour::handleScanResult(int rssi)
{
    if (rssi != 0)
    {                                                    // Assuming 0 means no valid RSSI
        auto blinkDelay = map(rssi, -100, 0, 1000, 100); // Adjust the range as needed
        m_blinker.setBlinkDelay(blinkDelay);
    }
}
