#include <gtest/gtest.h>
#include "./../src/AdxIO4ButtonRing.h"
#include "./../src/ConnectionProperties.h"
#include <gmock/gmock.h>

// Mocking callback functions
class MockCallbacks
{
public:
    MOCK_METHOD(void, OnDataReceived, (const uint8_t *, size_t), ());
    MOCK_METHOD(void, OnError, (), ());
};

TEST(AdxIO4ButtonRingTest, Connect_Success_Disconnect_Success)
{
    ConnectionProperties connectionProperties;
    AdxIO4ButtonRing obj{connectionProperties.product_id, connectionProperties.vender_id, connectionProperties.buffer_size};
    auto result = obj.connect();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Allow some buffer time for the device to initialize
    EXPECT_TRUE(result) << "Expected successful connection to the HID device.";
    result = obj.disconnect();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Allow some buffer time for the device to initialize
    EXPECT_TRUE(result) << "Expect disconnect to be successful";
}

TEST(AdxIO4ButtonRingTest, Read_ValidData)
{
    ConnectionProperties connectionProperties;
    AdxIO4ButtonRing obj{connectionProperties.product_id, connectionProperties.vender_id, connectionProperties.buffer_size};
    EXPECT_TRUE(obj.isConnected()) << "Expect device to be disconnected successful";
    EXPECT_TRUE(obj.connect());

    auto mock = std::make_unique<MockCallbacks>();
    // ignore memory leak warning
    testing::Mock::AllowLeak(mock.get());

    // Set expectation that OnDataReceived will be called
    EXPECT_CALL(*mock, OnDataReceived(testing::_, testing::_))
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Invoke([](const uint8_t *data, size_t size)
                                        {
                                            ASSERT_NE(data, nullptr);
                                            ASSERT_GT(size, 0);
                                            // You can add further assertions here if needed
                                        }));

    // Set expectation that OnError will NOT be called
    EXPECT_CALL(*mock, OnError())
        .Times(0); // No errors expected.

    // Start reading with the mock callbacks
    obj.read(
        [&](const uint8_t *data, size_t size)
        {
            mock->OnDataReceived(data, size);
        },
        [&]()
        {
            mock->OnError();
        });

    // Simulate reading; you might need to introduce a small sleep or wait for the thread.
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(obj.isConnected()) << "Expect device to be connected successfully";
    EXPECT_TRUE(obj.disconnect()) << "Expect disconnect to be successful";
}