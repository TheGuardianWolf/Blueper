#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <memory>
#include <Arduino.h>

class Advertisement : public IAdvertisement
{
public:
    Advertisement();
    void start() override;

    friend class BLECallbacks;

private:
    class BLECallbacks : public BLEServerCallbacks //, public BLESecurityCallbacks
    {
    public:
        void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;
        void onDisconnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;

        // bool onSecurityRequest() override;
        // void onAuthenticationComplete(esp_ble_auth_cmpl_t details) override;
        // uint32_t onPassKeyRequest() override { return 0; }
        // void onPassKeyNotify(uint32_t pass_key) override {}
        // bool onConfirmPIN(uint32_t pin) override { return false; }
    };

    // Shared Resources
    BLEServer *m_pServer;
    BLEAdvertising *m_pAdvertising;

    // Our own service
    BLEService *m_pService;
    BLECharacteristic *m_pCharacteristic;
    const std::unique_ptr<BLECallbacks> m_pCallbacks;
    BLESecurity m_security;

    // Device management
    bool m_paired = false;
};
