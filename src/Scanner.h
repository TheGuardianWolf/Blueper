#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <memory>

class Scanner : public IScanner
{
public:
    Scanner();
    void start() override;
    void update() override;
    int getRSSI() const override;
    void setRSSI(const int rssi) override;

private:
    static Scanner *s_registeredScanner;
    static void onScanComplete(BLEScanResults results);

    class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        AdvertisedDeviceCallbacks(const std::string &uuid, Scanner &scanner);
        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:
        std::string m_targetUUID;
        Scanner &m_scanner;
    };

    BLEScan *m_pBLEScan;
    const std::unique_ptr<AdvertisedDeviceCallbacks> m_pCallbacks;

    int m_latestRSSI;
};
