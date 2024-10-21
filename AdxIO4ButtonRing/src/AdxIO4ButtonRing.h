#ifndef ADXIO4BUTTONRING_H
#define ADXIO4BUTTONRING_H

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include "../../Library/hidapi/hidapi/hidapi.h"
#include "ConnectionProperties.h"

class DLL_EXPORT AdxIO4ButtonRing
{
private:
    std::unique_ptr<hid_device, decltype(&hid_close)> device;
    std::atomic<bool> reading; // Used to control the read loop
    std::thread readThread;
    std::unique_ptr<uint8_t[]> buffer;
    std::unique_ptr<uint8_t[]> previousState;
    std::mutex connectionMutex;
    const ConnectionProperties connectionProperties;

    void readLoop(std::function<void(const uint8_t *, size_t)> dataCallback,
                  std::function<void()> errorCallback);

public:
    AdxIO4ButtonRing(int vender_id, int product_id, int buffer_size)
        : device(nullptr, hid_close), reading(false),
          connectionProperties{vender_id, product_id, buffer_size}
    {
        buffer = std::make_unique<uint8_t[]>(buffer_size);
        previousState = std::make_unique<uint8_t[]>(buffer_size);
    }

    ~AdxIO4ButtonRing();

    bool isConnected();
    bool connect();
    void read(std::function<void(const uint8_t *, size_t)> dataCallback,
              std::function<void()> errorCallback);
    bool disconnect();
};

#endif // ADXIO4BUTTONRING_H