#include "Advertisement.h"
#include "Defines.h"

Advertisement::Advertisement(const std::string &uuid, uint16_t major, uint16_t minor, int8_t txPower)
{
    BLEDevice::init("Pinger");
    BLEServer *pServer = BLEDevice::createServer();
    pService = pServer->createService(BLEUUID(uuid));

    pCharacteristic = pService->createCharacteristic(
        BLEUUID(static_cast<uint16_t>(0x2A37)),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    pService->start();

    BLEAdvertisementData advertisementData;
    BLEAdvertisementData scanResponseData;

    // advertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

    // std::string serviceData;
    // serviceData += static_cast<char>(26);   // Length
    // serviceData += static_cast<char>(0xFF); // Type
    // serviceData += BLEUUID(uuid).toString();
    // advertisementData.addData(serviceData);

    scanResponseData.setName("FavPinger");
    advertisementData.setAppearance(512); // Generic Tag

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->setScanResponseData(scanResponseData);
}

void Advertisement::start()
{
    pAdvertising->start();
}
