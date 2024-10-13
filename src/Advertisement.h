#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class Advertisement : public IAdvertisement
{
public:
    Advertisement(const std::string &uuid, uint16_t major, uint16_t minor, int8_t txPower);
    void start() override;

private:
    BLEAdvertising *m_pAdvertising;
    BLEService *m_pService;
    BLECharacteristic *m_pCharacteristic;
};