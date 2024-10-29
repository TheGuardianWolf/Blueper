#include "Advertisement.h"
#include "Defines.h"
#include <Arduino.h>

// Singleton
Advertisement &Advertisement::create()
{
    static Advertisement instance;

    return instance;
}

void Advertisement::start(ITiming *pTiming)
{
    m_pTiming = pTiming;

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
    m_pCharacteristicTopic = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_TOPIC_ID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    m_pCharacteristicSetTopic = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_SET_TOPIC_ID,
        BLECharacteristic::PROPERTY_WRITE);

    m_pCharacteristicSetDeviceName = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_SET_DEVICE_NAME_ID,
        BLECharacteristic::PROPERTY_WRITE);

    m_pCharacteristicIsConfigurable = m_pService->createCharacteristic(
        BLUEPER_CHARACTERISTIC_IS_CONFIGURABLE_ID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    m_pCharacteristicSetTopic->setCallbacks(m_pCallbacks.get());
    m_pCharacteristicSetDeviceName->setCallbacks(m_pCallbacks.get());

    // Eventually save this into mem
    m_pCharacteristicTopic->setValue(DEFAULT_BLUEPER_VALUE);

    uint8_t TRUE = 1;
    m_pCharacteristicIsConfigurable->setValue(&TRUE, 1);

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

void Advertisement::loop()
{
    auto currentMs = m_pTiming->getTimestamp();
    if (currentMs >= 60000 && m_configurable)
    {
        Serial.println("Disabling configuration!");
        m_configurable = false;

        uint8_t FALSE = 0;
        m_pCharacteristicIsConfigurable->setValue(&FALSE, 1);
        m_pCharacteristicIsConfigurable->notify();
    }
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

void Advertisement::ServerCallbacks::onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param)
{
    auto uuid = pCharacteristic->getUUID();
    Serial.printf("Received characteristic write to: %s\n", uuid.toString().c_str());

    if (!m_advertisement.m_configurable)
    {
        Serial.println("Write rejected, configuration not allowed at this time");
        // Clear user input
        pCharacteristic->setValue(nullptr, 0);
        return;
    }

    auto value = pCharacteristic->getValue();
    if (uuid.equals(BLUEPER_CHARACTERISTIC_SET_TOPIC_ID))
    {
        auto *topicChar = m_advertisement.m_pCharacteristicTopic;
        topicChar->setValue(value);
        topicChar->notify();
    }
    else if (uuid.equals(BLUEPER_CHARACTERISTIC_SET_DEVICE_NAME_ID))
    {
        Serial.printf("Prentend device name changed to: %s\n", value.c_str());
        pCharacteristic->notify();
    }

    // Clear user input
    pCharacteristic->setValue(nullptr, 0);
}
