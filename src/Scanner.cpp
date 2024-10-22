#include <Arduino.h>
#include "Scanner.h"
#include "Defines.h"

// Singleton
Scanner &Scanner::create()
{
    static Scanner instance;

    return instance;
}

void Scanner::onScanComplete(BLEScanResults results)
{
    Serial.println("Scan completed");
    auto &scanner = create();
    scanner.update();
}

Scanner::Scanner()
    : m_latestRSSI(0), m_pBLEScan(BLEDevice::getScan())
{
}

void Scanner::start()
{
    m_pCallbacks = std::make_unique<AdvertisedDeviceCallbacks>(BLUEPER_SERVICE_ID, *this);
    m_pBLEScan->setAdvertisedDeviceCallbacks(m_pCallbacks.get());
    update();
}

void Scanner::update()
{
    Serial.println("Scan started");
    m_pBLEScan->start(5, &onScanComplete, false);
}

int Scanner::getRSSI() const
{
    return m_latestRSSI;
}

void Scanner::setRSSI(const int rssi)
{
    m_latestRSSI = rssi;
}

Scanner::AdvertisedDeviceCallbacks::AdvertisedDeviceCallbacks(const BLEUUID &uuid, Scanner &scanner)
    : m_targetUUID(uuid), m_scanner(scanner) {}

void Scanner::AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(m_targetUUID))
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
