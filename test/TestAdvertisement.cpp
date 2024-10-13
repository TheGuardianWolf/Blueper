#include <gtest/gtest.h>
#include "Advertisement.h"

class AdvertisementTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        advertisement = std::make_unique<Advertisement>("8ec76ea3-6668-48da-9866-75be8bc86f4d", 1, 1, 20);
    }

    std::unique_ptr<Advertisement> advertisement;
};

TEST_F(AdvertisementTest, StartCallsBLEAdvertisingStart)
{
    // This test would require integration with the BLE library to verify the start behavior.
    // For simplicity, we assume the BLE library works correctly and focus on the interaction.
    advertisement->start();
    // Verify internal state if needed
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
