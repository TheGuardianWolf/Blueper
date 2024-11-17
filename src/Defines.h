#pragma once

const std::string BLUEPER_NAME = "Blueper";
const BLEUUID BLUEPER_SERVICE_ID("4628f6d1-9c20-4624-b0ac-dbaa5f980678");
const BLEUUID BLUEPER_CHARACTERISTIC_TOPIC_ID("06e60583-1866-42cf-aeff-70cad32b727b");
const BLEUUID BLUEPER_CHARACTERISTIC_SET_TOPIC_ID("06e60583-1866-42cf-aeff-70cad32b727c");
const BLEUUID BLUEPER_CHARACTERISTIC_SET_DEVICE_NAME_ID("06e60583-1866-42cf-aeff-70cad32b727d");
const BLEUUID BLUEPER_CHARACTERISTIC_IS_CONFIGURABLE_ID("06e60583-1866-42cf-aeff-70cad32b727e");
constexpr uint8_t LED_PIN = 2; // Change this to the pin connected to your LED
constexpr uint32_t LOOP_TIME = 50;
constexpr uint32_t SCAN_DURATION = 1;

const std::string DEFAULT_BLUEPER_VALUE = "Blueper";
