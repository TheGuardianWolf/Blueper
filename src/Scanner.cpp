#include <Arduino.h>
#include "Scanner.h"

/*STATIC*/ void Scanner::onScanComplete(BLEScanResults results)
{
}

Scanner::Scanner(const std::string &uuid)
    : m_pCallbacks(std::make_unique<AdvertisedDeviceCallbacks>(uuid, *this)), m_latestRSSI(0), m_pBLEScan(BLEDevice::getScan())
{
    m_pBLEScan->setAdvertisedDeviceCallbacks(m_pCallbacks.get());
}

void Scanner::start()
{
    m_pBLEScan->start(5, &onScanComplete, true); // Scan for 5 seconds, restart automatically
}

int Scanner::getLatestRSSI() const
{
    return m_latestRSSI;
}

Scanner::AdvertisedDeviceCallbacks::AdvertisedDeviceCallbacks(const std::string &uuid, Scanner &scanner)
    : targetUUID(uuid), scanner(scanner) {}

void Scanner::AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(BLEUUID(targetUUID)))
    {
        Serial.print("Beacon found: ");
        Serial.println(advertisedDevice.toString().c_str());

        if (advertisedDevice.haveRSSI())
        {
            scanner.latestRSSI = advertisedDevice.getRSSI();
            Serial.print("RSSI: ");
            Serial.println(scanner.latestRSSI);
        }
    }
}
