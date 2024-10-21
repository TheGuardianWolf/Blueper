#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class Advertisement : public IAdvertisement
{
public:
    Advertisement();
    void start() override;

private:
    BLEServer *m_pServer;
    BLEAdvertising *m_pAdvertising;
    BLEService *m_pService;

    BLESecurity m_security;
    BLECharacteristic m_characteristic;
};
