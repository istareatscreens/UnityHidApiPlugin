#ifndef ADXIO4BUTTONRING_H
#define ADXIO4BUTTONRING_H

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <memory>
#include "../../Library/hidapi/hidapi/hidapi.h"

class DLL_EXPORT AdxIO4ButtonRing
{
private:
    std::unique_ptr<hid_device> *handle;

public:
    bool connect();
    void read();
    bool disconnect();
    AdxIO4ButtonRing() {};
    ~AdxIO4ButtonRing() {};
};

#endif // ADXHIDBUTTONRING_H