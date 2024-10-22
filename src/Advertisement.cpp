#include "Advertisement.h"
#include "Defines.h"
#include <Arduino.h>

// Singleton
Advertisement &Advertisement::create()
{
    static Advertisement instance;

    return instance;
}

void Advertisement::start()
{
    BLEDevice::init(BLUEPER_NAME);
    // Bonding should be "Just Works" or push button
    m_security.setCapability(ESP_IO_CAP_NONE);
    m_security.setAuthenticationMode(ESP_LE_AUTH_NO_BOND);

    // Initialise device and BT service
    m_pServer = BLEDevice::createServer();
    m_pCallbacks = std::make_unique<ServerCallbacks>(*this);
    BLEDevice::setSecurityCallbacks(m_pCallbacks.get());
    m_pServer->setCallbacks(m_pCallbacks.get());
    m_pService = m_pServer->createService(BLUEPER_SERVICE_ID);
    m_pCharacteristic = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_ID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    // Write permissions are removed after 3 minutes

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

    m_pService->start();
    m_pAdvertising->start();

    Serial.println("Advertisement started");
}

void Advertisement::ServerCallbacks::onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Serial.println("Device connected");
    pServer->startAdvertising();
}

void Advertisement::ServerCallbacks::onDisconnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Serial.println("Device disconnected");
    pServer->startAdvertising();
}
