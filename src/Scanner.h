#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <memory>
#include <list>
#include <mutex>

class Scanner : public IScanner
{
public:
    static Scanner &create();

    // Prevent creation
    Scanner(Scanner const &) = delete;
    void operator=(Scanner const &) = delete;

    void start() override;
    int getRSSI() const override;

    friend class AdvertisedDeviceCallbacks;

private:
    struct ScannedDevice
    {
        BLEAddress m_address;
        std::string m_name = "";
        int m_rssi = 0;
        unsigned long m_lastIterationSeen = 0;
    };

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

    std::mutex m_lock;
    BLEScan *m_pBLEScan;
    std::unique_ptr<AdvertisedDeviceCallbacks> m_pCallbacks;

    unsigned long m_scanIteration = 0;
    std::list<ScannedDevice> m_scannedDevices;

    void update() override;
};
