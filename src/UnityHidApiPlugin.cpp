#include "UnityHidApiPlugin.h"
#include "ConnectionProperties.h"
#include <iostream>

bool UnityHidApiPlugin::connect()
{

    // Attempt to open the HID device using the vendor and product IDs
    // hid_device *raw_device = hid_open(connection_properties.vender_id, connection_properties.product_id, nullptr);

    // windows often installs duplicate drivers for some reason
    // so we need to find the correct device out of potential duplicates

    auto devices_info_linked_list = hid_enumerate(connectionProperties.vender_id, connectionProperties.product_id);
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
        if (hid_set_nonblocking(raw_device, 1) < 0)
        {
            // TODO: Non blocking isn't supported well this is awkward potential issues with some devices
        }

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

    // Store the device in the unique pointer
    device.reset(raw_device);

    return true;
}

void UnityHidApiPlugin::read(
    std::function<void(const uint8_t *, size_t)> dataCallback,
    std::function<void()> errorCallback)
{
    if (!device)
    {
        return;
    }

    reading.store(true);

    // Start the read loop in a separate thread to avoid blocking
    readThread = std::thread(
        [this, dataCallback, errorCallback]()
        { readLoop(dataCallback, errorCallback); });
}

void UnityHidApiPlugin::readLoop(
    std::function<void(const uint8_t *, size_t)> dataCallback,
    std::function<void()> eventCallback)
{
    hid_device *rawDevice = device.get();
    uint8_t *rawBuffer = buffer.get();
    uint8_t *prevState = previousState.get();
    const size_t bufferSize = connectionProperties.buffer_size;

    if (!rawDevice || !rawBuffer || !prevState)
    {
        eventCallback();
        return;
    }

    try
    {
        while (reading.load())
        {
            // Read from the HID device into the buffer
            int bytesRead = hid_read(rawDevice, rawBuffer, bufferSize);

            if (bytesRead == bufferSize)
            {
                // If the data equals previous state no processing required
                if (0 == std::memcmp(rawBuffer, prevState, bytesRead))
                {
                    // prevent infinite loop stuck on zero data returned from device
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    continue;
                }

                std::memcpy(prevState, rawBuffer, bytesRead);
                dataCallback(rawBuffer, bytesRead);
            }
            else if (-1 == bytesRead)
            {
                disconnect();
                eventCallback();
                break;
            }
        }
    }
    catch (...)
    {
        disconnect();
        eventCallback();
    }
}

bool UnityHidApiPlugin::disconnect()
{
    if (reading.load())
    {
        // stop reading and kill thread
        reading.store(false);

        if (readThread.joinable())
        {
            readThread.join();
        }
    }

    try
    {
        hid_close(device.get());
    }
    catch (...)
    {
        // Its ok if it fails here
    }
    device.reset();

    return true;
}

bool UnityHidApiPlugin::isConnected()
{
    std::lock_guard<std::mutex> lock(connectionMutex);
    return reading.load();
}

UnityHidApiPlugin::~UnityHidApiPlugin()
{
    std::lock_guard<std::mutex> lock(connectionMutex);
    disconnect();
}