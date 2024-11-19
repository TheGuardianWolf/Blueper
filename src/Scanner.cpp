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

    std::lock_guard lock{scanner.m_lock};
    // If scan iteration > 2, remove it
    scanner.m_scannedDevices.remove_if([&scanner](const ScannedDevice &sd)
                                       { return sd.m_lastIterationSeen + 2 < scanner.m_scanIteration; });
    scanner.m_scanIteration++;
}

Scanner::Scanner()
    : m_pBLEScan(BLEDevice::getScan())
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
    m_pBLEScan->start(SCAN_DURATION, &onScanComplete, false);
}

int Scanner::getRSSI()
{
    std::lock_guard lock{m_lock};

    if (!m_scannedDevices.size())
    {
        return 0;
    }

    auto iter = std::max_element(m_scannedDevices.begin(), m_scannedDevices.end(), [](const ScannedDevice &sdA, const ScannedDevice &sdB)
                                 { return sdA.m_rssi < sdB.m_rssi; });
    return iter->m_rssi;
}

Scanner::AdvertisedDeviceCallbacks::AdvertisedDeviceCallbacks(const BLEUUID &uuid, Scanner &scanner)
    : m_targetUUID(uuid), m_scanner(scanner) {}

void Scanner::AdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(m_targetUUID))
    {
        if (advertisedDevice.haveRSSI() && advertisedDevice.haveTXPower())
        {
            auto name = advertisedDevice.toString();
            auto address = advertisedDevice.getAddress();
            auto rssi = advertisedDevice.getRSSI();

            Serial.print("Beacon found: ");
            Serial.println(name.data());
            Serial.print("RSSI: ");
            Serial.println(rssi);

            std::lock_guard lock{m_scanner.m_lock};

            if (auto iter = std::find_if(m_scanner.m_scannedDevices.begin(), m_scanner.m_scannedDevices.end(), [&address](const ScannedDevice &sd)
                                         { return address.equals(sd.m_address); });
                iter != m_scanner.m_scannedDevices.end())
            {
                iter->m_lastIterationSeen = m_scanner.m_scanIteration;
                iter->m_rssi = rssi;
            }
            else
            {
                m_scanner.m_scannedDevices.push_back(ScannedDevice{
                    .m_address = address,
                    .m_name = name,
                    .m_rssi = rssi,
                    .m_lastIterationSeen = m_scanner.m_scanIteration});
            }
        }
    }
}
