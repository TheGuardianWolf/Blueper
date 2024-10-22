#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <memory>

class Scanner : public IScanner
{
public:
    static Scanner &create();

    // Prevent creation
    Scanner(Scanner const &) = delete;
    void operator=(Scanner const &) = delete;

    void start() override;
    void update() override;
    int getRSSI() const override;
    void setRSSI(const int rssi) override;

    friend class AdvertisedDeviceCallbacks;

private:
    class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        AdvertisedDeviceCallbacks(const BLEUUID &uuid, Scanner &scanner);
        void onResult(BLEAdvertisedDevice advertisedDevice) override;

    private:
        const BLEUUID m_targetUUID;
        Scanner &m_scanner;
    };

    static void onScanComplete(BLEScanResults results);

    Scanner();

    BLEScan *m_pBLEScan;
    std::unique_ptr<AdvertisedDeviceCallbacks> m_pCallbacks;

    int m_latestRSSI;
};
