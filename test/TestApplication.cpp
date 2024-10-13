#include <gtest/gtest.h>
#include "Application.h"
#include "MockAdvertisement.h"
#include "MockScanner.h"
#include "MockBlinker.h"

class ApplicationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockAdvertisement = std::make_unique<MockAdvertisement>();
        mockScanner = std::make_unique<MockScanner>();
        mockBlinker = std::make_unique<MockBlinker>();
        app = std::make_unique<Application>(*mockAdvertisement, *mockScanner, *mockBlinker);
    }

    std::unique_ptr<MockAdvertisement> mockAdvertisement;
    std::unique_ptr<MockScanner> mockScanner;
    std::unique_ptr<MockBlinker> mockBlinker;
    std::unique_ptr<Application> app;
};

TEST_F(ApplicationTest, RunStartsAdvertisementAndScanner)
{
    EXPECT_CALL(*mockAdvertisement, start()).Times(1);
    EXPECT_CALL(*mockScanner, start()).Times(1);

    // Run the application in a separate thread to avoid blocking
    std::thread appThread(&{ app->run(); });

    // Allow some time for the application to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Stop the application thread
    appThread.detach();
}

TEST_F(ApplicationTest, HandleScanResultCallsBlinker)
{
    EXPECT_CALL(*mockScanner, getLatestRSSI()).WillOnce(Return(-50));
    EXPECT_CALL(*mockBlinker, setBlinkDelay(::testing::_)).Times(1);
    EXPECT_CALL(*mockBlinker, blink()).Times(1);

    app->run();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
