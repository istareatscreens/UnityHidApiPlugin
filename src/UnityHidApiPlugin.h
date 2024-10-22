#ifndef ADXIO4BUTTONRING_H
#define ADXIO4BUTTONRING_H

#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include "../library/hidapi/hidapi/hidapi.h"
#include "ConnectionProperties.h"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class DLL_EXPORT UnityHidApiPlugin
{
private:
    std::unique_ptr<hid_device, decltype(&hid_close)> device;
    std::atomic<bool> reading; // Used to control the read loop
    std::thread readThread;
    std::unique_ptr<uint8_t[]> buffer;
    std::unique_ptr<uint8_t[]> previousBuffer;
    std::mutex connectionMutex;
    const ConnectionProperties connectionProperties;

    void readLoop(
        std::function<void(const uint8_t *)> dataCallback,
        std::function<void(std::string)> errorCallback);
    void clearBuffers();

public:
    UnityHidApiPlugin(int vendor_id, int product_id, int buffer_size)
        : device(nullptr, hid_close), reading(false),
          connectionProperties{vendor_id, product_id, buffer_size}
    {
        buffer = std::make_unique<uint8_t[]>(buffer_size);
        previousBuffer = std::make_unique<uint8_t[]>(buffer_size);
    }

    ~UnityHidApiPlugin();

    bool isConnected();
    bool isReading();
    bool connect();
    void read(std::function<void(const uint8_t *)> dataCallback,
              std::function<void(std::string)> errorCallback);
    bool disconnect();
};

#endif // ADXIO4BUTTONRING_H