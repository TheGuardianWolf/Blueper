#include <Arduino.h>
#include "Scanner.h"
#include "Defines.h"

/*STATIC*/ void Scanner::onScanComplete(BLEScanResults results)
{
    if (s_registeredScanner == nullptr)
    {
        return;
    }

    s_registeredScanner->start();
}

Scanner::Scanner()
    : m_pCallbacks(std::make_unique<AdvertisedDeviceCallbacks>(BLUEPER_SERVICE_ID, *this)), m_latestRSSI(0), m_pBLEScan(BLEDevice::getScan())
{
    m_pBLEScan->setAdvertisedDeviceCallbacks(m_pCallbacks.get());
}

void Scanner::start()
{
    s_registeredScanner = this;
    m_pBLEScan->start(5, &onScanComplete, true);
}

int Scanner::getRSSI() const
{
    return m_latestRSSI;
}

void Scanner::setRSSI(const int rssi)
{
    m_latestRSSI = rssi;
}

Scanner::AdvertisedDeviceCallbacks::AdvertisedDeviceCallbacks(const std::string &uuid, Scanner &scanner)
    : m_targetUUID(uuid), m_scanner(scanner) {}

void Scanner::AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(BLEUUID(m_targetUUID)))
    {
        Serial.print("Beacon found: ");
        Serial.println(advertisedDevice.toString().c_str());

        if (advertisedDevice.haveRSSI())
        {
            auto rssi = advertisedDevice.getRSSI();
            m_scanner.setRSSI(rssi);
            Serial.print("RSSI: ");
            Serial.println(rssi);
        }
    }
}
