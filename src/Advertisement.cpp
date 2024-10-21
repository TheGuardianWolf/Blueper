#include "Advertisement.h"
#include "Defines.h"
#include "Arduino.h"

Advertisement::Advertisement()
    : m_characteristic(BLUEPER_CHARACTERISTIC_ID)
{
    BLEDevice::init(BLUEPER_NAME);
    // Bonding should be "Just Works" or push button
    m_security.setCapability(ESP_IO_CAP_NONE);

    // Initialise device and BT service
    m_pServer = BLEDevice::createServer();
    m_pService = m_pServer->createService(BLEUUID(BLUEPER_SERVICE_ID));

    // Configure chracteristic
    m_characteristic.setNotifyProperty(true);
    m_characteristic.setReadProperty(true);
    m_characteristic.setWriteProperty(true);
    m_characteristic.setAccessPermissions(ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE_ENCRYPTED);

    // Eventually save this into mem
    m_characteristic.setValue(DEFAULT_BLUEPER_VALUE);

    // Put the chracteristic in
    m_pService->addCharacteristic(&m_characteristic);

    BLEAdvertisementData scanResponseData;
    scanResponseData.setName(BLUEPER_NAME);

    m_pAdvertising = m_pServer->getAdvertising();
    m_pAdvertising->setScanResponseData(scanResponseData);
    m_pAdvertising->setAppearance(ESP_BLE_APPEARANCE_GENERIC_TAG);
    m_pAdvertising->addServiceUUID(BLUEPER_SERVICE_ID);
}

void Advertisement::start()
{
    m_pService->start();
    m_pAdvertising->start();

    Serial.println("Advertisement started");
}
