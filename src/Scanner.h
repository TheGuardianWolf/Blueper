#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <memory>

class Scanner : public IScanner
{
public:
    Scanner(const std::string &uuid);
    void start() override;
    int getLatestRSSI() const override;

private:
    static void onScanComplete(BLEScanResults results);

    class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        AdvertisedDeviceCallbacks(const std::string &uuid, Scanner &scanner);
        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:
        std::string targetUUID;
        Scanner &scanner;
    };

    BLEScan *m_pBLEScan;
    const std::unique_ptr<AdvertisedDeviceCallbacks> m_pCallbacks;

    int m_latestRSSI;
};
