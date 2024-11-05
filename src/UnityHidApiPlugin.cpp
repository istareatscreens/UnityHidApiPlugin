#include "UnityHidApiPlugin.h"
#include "ConnectionProperties.h"
#include <iostream>

bool UnityHidApiPlugin::connect()
{
    // clear buffers to prevent side effects
    clearBuffers();

    // Attempt to open the HID device using the vendor and product IDs
    // hid_device *raw_device = hid_open(connection_properties.vendor_id, connection_properties.product_id, nullptr);

    // windows often installs duplicate drivers for some reason
    // so we need to find the correct device out of potential duplicates

    auto devices_info_linked_list = hid_enumerate(connectionProperties.vendor_id, connectionProperties.product_id);
    if (nullptr == devices_info_linked_list)
    {
        return false;
    }

    hid_device *raw_device = nullptr;
    while (devices_info_linked_list)
    {
        // try to read from the device
        // Set non-blocking mode for the device
        raw_device = hid_open_path(devices_info_linked_list->path);
        // wait some time for connection to establish
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Do a read to make sure the device is actually connected
        int bytesRead = hid_read(raw_device, buffer.get(), connectionProperties.buffer_size);
        if (-1 != bytesRead)
        {
            // we got data
            break;
        }

        hid_close(raw_device);
        raw_device = nullptr;
        devices_info_linked_list = devices_info_linked_list->next;
    }

    hid_free_enumeration(devices_info_linked_list);

    if (!raw_device)
    {
        // no device connected
        return false;
    }

    if (hid_set_nonblocking(raw_device, 1) < 0)
    {
        // TODO: Non blocking isn't supported well this is awkward potential issues with some devices
    }

    // Store the device in the unique pointer
    device.reset(raw_device);

    return true;
}

void UnityHidApiPlugin::read(
    const std::function<void(const uint8_t *)> dataCallback,
    const std::function<void(std::string)> errorCallback)
{
    if (!device)
    {
        return;
    }

    // clear buffers to prevent side effects
    clearBuffers();

    reading.store(true);

    // Start the read loop in a separate thread to avoid blocking
    if (0 < connectionProperties.polling_rate_ms)
    {
        readThread = std::thread(
            [this, dataCallback, errorCallback]()
            { readLoopPolling(dataCallback, errorCallback); });
        return;
    }
    readThread = std::thread(
        [this, dataCallback, errorCallback]()
        { readLoop(dataCallback, errorCallback); });
}

void UnityHidApiPlugin::readLoopPolling(
    const std::function<void(const uint8_t *)> dataCallback,
    const std::function<void(std::string)> eventCallback

)
{
    hid_device *raw_device = device.get();
    uint8_t *raw_buffer = buffer.get();
    uint8_t *previous_state = previousBuffer.get();
    uint8_t *external_buffer = externalBuffer.get();
    // According to standard HID devices always
    // return consistent packet sizes (I could be wrong)
    const size_t bufferSize = connectionProperties.buffer_size;
    const size_t left_bytes_to_truncate = connectionProperties.left_bytes_to_truncate;
    const size_t bytes_to_read = connectionProperties.bytes_to_read;
    const size_t polling_rate_ms = connectionProperties.polling_rate_ms;

    if (!raw_device || !raw_buffer || !previous_state)
    {
        eventCallback("Buffers were not instantiated there is something wrong with the plugin");
        return;
    }

    try
    {
        while (reading.load(std::memory_order_relaxed))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(polling_rate_ms));

            int bytesRead = hid_read(raw_device, raw_buffer, bufferSize);

            // -1 indicates a read error exit and disconnect
            if (-1 == bytesRead)
            {
                break;
            }

            // shift the raw_buffer to truncate it so we only compare the section that matters
            uint8_t *data_to_process = raw_buffer + left_bytes_to_truncate;

            // If the data equals previous state no processing required
            if (bytesRead != bufferSize ||
                0 == std::memcmp(
                         data_to_process,
                         previous_state,
                         bytes_to_read

                         ))
            {
                continue;
            }

            std::memcpy(previous_state, data_to_process, bytes_to_read);
            std::memcpy(external_buffer, data_to_process, bytes_to_read);
            // the caller must store how many bytes they chose to read
            dataCallback(external_buffer);
        }
    }
    catch (...)
    {
        eventCallback("An error occured while reading disconnecting...");
        disconnect();
    }
}

void UnityHidApiPlugin::readLoop(
    const std::function<void(const uint8_t *)> dataCallback,
    const std::function<void(std::string)> eventCallback

)
{
    hid_device *raw_device = device.get();
    uint8_t *raw_buffer = buffer.get();
    uint8_t *previous_state = previousBuffer.get();
    uint8_t *external_buffer = externalBuffer.get();
    // According to standard HID devices always
    // return consistent packet sizes (I could be wrong)
    const size_t bufferSize = connectionProperties.buffer_size;
    const size_t left_bytes_to_truncate = connectionProperties.left_bytes_to_truncate;
    const size_t bytes_to_read = connectionProperties.bytes_to_read;

    if (!raw_device || !raw_buffer || !previous_state)
    {
        eventCallback("Buffers were not instantiated there is something wrong with the plugin");
        return;
    }

    try
    {
        while (reading.load(std::memory_order_relaxed))
        {
            int bytesRead = hid_read(raw_device, raw_buffer, bufferSize);

            // -1 indicates a read error exit and disconnect
            if (-1 == bytesRead)
            {
                break;
            }

            // shift the raw_buffer to truncate it so we only compare the section that matters
            uint8_t *data_to_process = raw_buffer + left_bytes_to_truncate;

            // If the data equals previous state no processing required
            if (bytesRead != bufferSize ||
                0 == std::memcmp(
                         data_to_process,
                         previous_state,
                         bytes_to_read

                         ))
            {
                // std::this_thread::sleep_for(std::chrono::milliseconds(3));
                continue;
            }

            std::memcpy(previous_state, data_to_process, bytes_to_read);
            std::memcpy(external_buffer, data_to_process, bytes_to_read);
            // the caller must store how many bytes they chose to read
            dataCallback(external_buffer);
        }
    }
    catch (...)
    {
        eventCallback("An error occured while reading disconnecting...");
        disconnect();
    }
}

void UnityHidApiPlugin::clearBuffers()
{
    buffer.reset(new uint8_t[connectionProperties.buffer_size]);
    previousBuffer.reset(new uint8_t[connectionProperties.bytes_to_read]);
    externalBuffer.reset(new uint8_t[connectionProperties.bytes_to_read]);
}

bool UnityHidApiPlugin::disconnect()
{
    stopReading();
    device.reset();

    return true;
}

bool UnityHidApiPlugin::isConnected()
{
    return nullptr != device.get();
}

bool UnityHidApiPlugin::isReading()
{
    std::lock_guard<std::mutex> lock(connectionMutex);
    return reading.load();
}

void UnityHidApiPlugin::stopReading()
{
    std::lock_guard<std::mutex> lock(connectionMutex);
    if (reading.load())
    {
        // stop reading and kill thread
        reading.store(false);
        if (readThread.joinable())
        {
            readThread.join();
        }
    }
}

UnityHidApiPlugin::~UnityHidApiPlugin()
{
    std::lock_guard<std::mutex> lock(connectionMutex);
    disconnect();
}