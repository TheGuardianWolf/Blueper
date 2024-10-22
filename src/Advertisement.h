#pragma once

#include "Interfaces.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <memory>
#include <Arduino.h>

/// @brief Singleton
class Advertisement : public IAdvertisement
{
public:
    static Advertisement &create();

    // Prevent creation
    Advertisement(Advertisement const &) = delete;
    void operator=(Advertisement const &) = delete;

    void start(ITiming *pTiming) override;
    void loop() override;

    friend class ServerCallbacks;

private:
    class ServerCallbacks
        : public BLEServerCallbacks,
          public BLESecurityCallbacks,
          public BLECharacteristicCallbacks
    {
    public:
        ServerCallbacks(Advertisement &advertisement)
            : m_advertisement(advertisement) {}

        // BLEServerCallbacks
        void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;
        void onDisconnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;

        // BLESecurityCallbacks
        bool onSecurityRequest() { return false; };
        void onAuthenticationComplete(esp_ble_auth_cmpl_t details) {};
        uint32_t onPassKeyRequest() override { return 0; }
        void onPassKeyNotify(uint32_t pass_key) override {}
        bool onConfirmPIN(uint32_t pin) override { return false; }

        // BLECharacteristicCallbacks
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param);

    private:
        Advertisement &m_advertisement;
    };

    Advertisement() {}

    std::unique_ptr<ServerCallbacks> m_pCallbacks;
    ITiming *m_pTiming;
    bool m_configurable = true;

    // Shared Resources
    BLEServer *m_pServer;
    BLEAdvertising *m_pAdvertising;
    BLESecurity m_security;

    // Our own service
    BLEService *m_pService;
    BLECharacteristic *m_pCharacteristicTopic;
    BLECharacteristic *m_pCharacteristicSetTopic;
    BLECharacteristic *m_pCharacteristicDeviceName;
};
