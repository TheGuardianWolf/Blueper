#include "Behaviour.h"
#include "Blinker.h"
#include <Arduino.h>

Behaviour::Behaviour(ITiming &timing, IAdvertisement &advertisement, IScanner &scanner, IBlinker &blinker)
    : m_timing(timing), m_advertisement(advertisement), m_scanner(scanner), m_blinker(blinker) {}

void Behaviour::start()
{
    m_advertisement.start(&m_timing);
    m_scanner.start();
    m_blinker.setBlinkDelay(Blinker::OFF);

    // Every second set the blink delay from rssi measurement
    m_blinkRefreshID = m_timing.scheduleEvent(
        [this]()
        {
            Serial.println("Checking RSSI...");
            auto pathLoss = m_scanner.getPathLoss();
            handleScanResult(pathLoss);
            return false;
        },
        1000, 0);
}

void Behaviour::handleScanResult(int pathLoss)
{
    if (pathLoss != 0)
    {                                                        // Assuming 0 means no valid RSSI
        auto blinkDelay = map(pathLoss, 100, 10, 3000, 100); // Adjust the range as needed
        if (pathLoss != m_currentPathLoss)
        {
            m_currentPathLoss = pathLoss;
            Serial.printf("Path loss is %d, setting delay to %d\n", pathLoss, blinkDelay);
            m_blinker.setBlinkDelay(blinkDelay);
        }
    }
    else
    {
        m_blinker.setBlinkDelay(0);
    }
}
