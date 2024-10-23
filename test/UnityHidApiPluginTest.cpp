#include <gtest/gtest.h>
#include "./../src/UnityHidApiPlugin.h"
#include "./../src/ConnectionProperties.h"
#include <gmock/gmock.h>
#include <PluginWrapper.h>

// Mocking callback functions
class MockCallbacks
{
public:
    MOCK_METHOD(void, OnDataReceived, (const uint8_t *), ());
    MOCK_METHOD(void, OnError, (std::string), ());
};

TEST(UnityHidApiPluginTest, Connect_Success_Disconnect_Success)
{
    ConnectionProperties connectionProperties;
    UnityHidApiPlugin obj{connectionProperties.vendor_id, connectionProperties.product_id, connectionProperties.buffer_size};
    auto result = obj.connect();
    EXPECT_TRUE(result) << "Expected successful connection to the HID device.";
    EXPECT_TRUE(obj.isConnected()) << "Expected successful connection to the HID device.";
    result = obj.disconnect();
    EXPECT_TRUE(result) << "Expect disconnect to be successful";
}

TEST(UnityHidApiPluginTest, Read_ValidData)
{
    ConnectionProperties connectionProperties;
    UnityHidApiPlugin obj{connectionProperties.vendor_id, connectionProperties.product_id, connectionProperties.buffer_size};
    EXPECT_FALSE(obj.isConnected()) << "Expect device to be disconnected";
    EXPECT_TRUE(obj.connect());
    EXPECT_FALSE(obj.isReading()) << "Expect device to not be reading";

    // Setup reading
    auto mock = std::make_unique<MockCallbacks>();

    // ignore memory leak warning
    testing::Mock::AllowLeak(mock.get());

    EXPECT_CALL(*mock, OnDataReceived(testing::_))
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Invoke([](const uint8_t *data)
                                        { ASSERT_NE(data, nullptr); }));

    EXPECT_CALL(*mock, OnError(testing::_))
        .Times(0); // No errors expected.

    obj.read(
        [&](const uint8_t *data)
        {
            mock->OnDataReceived(data);
        },
        [&](std::string message)
        {
            mock->OnError("Impossible to reach here");
        });

    // Simulate reading
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(obj.isReading()) << "Expect device to be reading";
    EXPECT_TRUE(obj.isConnected()) << "Expect device to be connected successfully";

    // End reading and disconnect
    EXPECT_TRUE(obj.disconnect()) << "Expect disconnect to be successful";

    EXPECT_FALSE(obj.isConnected()) << "Expect device to be disconnected";
    EXPECT_FALSE(obj.isReading()) << "Expect device to not be reading";
}

TEST(UnityHidApiPlugin, Test_Creation_Of_UnityHidApiPlugin_And_Connection_Through_Plugin_Interface)
{
    ConnectionProperties ConnectionProperties;
    auto objPtr = Initialize(ConnectionProperties.vendor_id, ConnectionProperties.product_id, ConnectionProperties.buffer_size);
    EXPECT_NE(objPtr, nullptr);

    EXPECT_TRUE(Connect(objPtr)) << "Expected successful connection to the HID device.";
    EXPECT_TRUE(IsConnected(objPtr)) << "Expected successful connection to the HID device.";

    /*
    // Setup reading
    auto mock = std::make_unique<MockCallbacks>();
    // ignore memory leak warning
    testing::Mock::AllowLeak(mock.get());

    // Prepare for reading
    EXPECT_CALL(*mock, OnDataReceived(testing::_))
        .Times(testing::AtLeast(1))
        .WillRepeatedly(testing::Invoke([](const uint8_t *data)
                                        { ASSERT_NE(data, nullptr); }));

    EXPECT_CALL(*mock, OnError(testing::_))
        .Times(0); // No errors expected.

        Read(objPtr, [&](const uint8_t *data)
             { mock->OnDataReceived(data); }, [&](std::string message)
             { mock->OnError("Impossible to reach here"); });
    */
}
