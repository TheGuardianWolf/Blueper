#include "Advertisement.h"
#include "Defines.h"

Advertisement::Advertisement(uint16_t major, uint16_t minor, int8_t txPower)
{
    m_pServer = BLEDevice::createServer();
    m_pService = m_pServer->createService(BLEUUID(BLUEPER_SERVICE_ID));

    m_pCharacteristic = m_pService->createCharacteristic(
        BLEUUID(static_cast<uint16_t>(BLUEPER_CHARACTERISTIC_ID)),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    BLEAdvertisementData advertisementData;
    BLEAdvertisementData scanResponseData;

    scanResponseData.setName(BLUEPER_NAME);
    advertisementData.setAppearance(512); // Generic Tag

    m_pAdvertising = m_pServer->getAdvertising();
    m_pAdvertising->setAdvertisementData(advertisementData);
    m_pAdvertising->setScanResponseData(scanResponseData);
}

void Advertisement::start()
{
    m_pService->start();
    m_pAdvertising->start();
}
