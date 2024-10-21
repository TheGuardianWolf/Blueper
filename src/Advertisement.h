#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <memory>

class Advertisement : public IAdvertisement
{
public:
    Advertisement();
    void start() override;

private:
    class BLECallbacks : public BLEServerCallbacks
    {
    public:
        BLECallbacks(Advertisement &advertisement)
            : m_advertisement(advertisement)
        {
        }

    private:
        Advertisement &m_advertisement;
    };

    BLEServer *m_pServer;
    BLEAdvertising *m_pAdvertising;
    BLEService *m_pService;
    BLECharacteristic *m_pCharacteristic;

    const std::unique_ptr<BLECallbacks> m_pCallbacks;
    BLESecurity m_security;
};
