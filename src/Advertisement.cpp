#include "Advertisement.h"
#include "Defines.h"
#include <Arduino.h>

Advertisement::Advertisement()
    : m_pCallbacks(std::make_unique<BLECallbacks>())
{
    BLEDevice::init(BLUEPER_NAME);
    // Bonding should be "Just Works" or push button
    m_security.setCapability(ESP_IO_CAP_NONE);

    // Initialise device and BT service
    m_pServer = BLEDevice::createServer();
    // BLEDevice::setSecurityCallbacks(m_pCallbacks.get());
    m_pServer->setCallbacks(m_pCallbacks.get());
    m_pService = m_pServer->createService(BLUEPER_SERVICE_ID);
    m_pCharacteristic = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_ID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    m_pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE_ENCRYPTED);

    // Eventually save this into mem
    m_pCharacteristic->setValue(DEFAULT_BLUEPER_VALUE);

    BLEAdvertisementData advertisementData;
    advertisementData.setAppearance(ESP_BLE_APPEARANCE_GENERIC_TAG); // Generic Tag
    // Use crc16 hash here to help ping devices
    // advertisementData.setManufacturerData

    m_pAdvertising = m_pServer->getAdvertising();
    m_pAdvertising->setAdvertisementData(advertisementData);
    m_pAdvertising->setScanResponse(true);
    m_pAdvertising->addServiceUUID(BLUEPER_SERVICE_ID);
}

void Advertisement::start()
{
    m_pService->start();
    m_pAdvertising->start();

    Serial.println("Advertisement started");
}

void Advertisement::BLECallbacks::onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Serial.println("Device connected");
    pServer->startAdvertising();
}

void Advertisement::BLECallbacks::onDisconnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Serial.println("Device disconnected");
    pServer->startAdvertising();
}

// bool Advertisement::BLECallbacks::onSecurityRequest()
// {
//     Serial.println("Pairing request");

//     // if (m_advertisement.m_paired)
//     // {
//     //     return false;
//     // }

//     // m_advertisement.m_paired = true;
//     return true;
// }

// void Advertisement::BLECallbacks::onAuthenticationComplete(esp_ble_auth_cmpl_t details)
// {
//     Serial.printf("Pairing complete: %s\n", details.success);
//     // m_advertisement.m_paired = details.success;
// }
