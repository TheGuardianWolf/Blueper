#include <gtest/gtest.h>
#include "Scanner.h"

class MockBLEScan : public BLEScan
{
public:
    MOCK_METHOD(void, start, (uint32_t duration, bool is_continue), (override));
};

class ScannerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockScan = new MockBLEScan();
        scanner = std::make_unique<Scanner>("8ec76ea3-6668-48da-9866-75be8bc86f4d", 2);
    }

    void TearDown() override
    {
        delete mockScan;
    }

    MockBLEScan *mockScan;
    std::unique_ptr<Scanner> scanner;
};

TEST_F(ScannerTest, StartCallsScanStart)
{
    EXPECT_CALL(*mockScan, start(5, true)).Times(1);
    scanner->start();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
