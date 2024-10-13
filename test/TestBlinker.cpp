#include <gtest/gtest.h>
#include "LEDBlinker.h"

class MockArduino
{
public:
    MOCK_METHOD(void, pinMode, (int pin, int mode));
    MOCK_METHOD(void, digitalWrite, (int pin, int value));
    MOCK_METHOD(void, delay, (int ms));
};

MockArduino *mockArduino;

void pinMode(int pin, int mode)
{
    mockArduino->pinMode(pin, mode);
}

void digitalWrite(int pin, int value)
{
    mockArduino->digitalWrite(pin, value);
}

void delay(int ms)
{
    mockArduino->delay(ms);
}

class LEDBlinkerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockArduino = new MockArduino();
        blinker = std::make_unique<LEDBlinker>(2);
    }

    void TearDown() override
    {
        delete mockArduino;
    }

    std::unique_ptr<LEDBlinker> blinker;
};

TEST_F(LEDBlinkerTest, SetBlinkDelay)
{
    blinker->setBlinkDelay(500);
    // Verify internal state if needed
}

TEST_F(LEDBlinkerTest, Blink)
{
    EXPECT_CALL(*mockArduino, digitalWrite(2, HIGH)).Times(1);
    EXPECT_CALL(*mockArduino, delay(1000)).Times(1);
    EXPECT_CALL(*mockArduino, digitalWrite(2, LOW)).Times(1);
    EXPECT_CALL(*mockArduino, delay(1000)).Times(1);

    blinker->blink();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
