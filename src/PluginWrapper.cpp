#include "PluginWrapper.h"
#include "UnityHidApiPlugin.h"

extern "C"
{
    DLL_EXPORT UnityHidApiPlugin *Initialize(int vendor_id, int product_id, int buffer_size)
    {
        return new UnityHidApiPlugin(vendor_id, product_id, buffer_size);
    }

    DLL_EXPORT void Dispose(UnityHidApiPlugin *obj)
    {
        if (obj)
        {
            delete obj;
        }
    }

    DLL_EXPORT bool Connect(UnityHidApiPlugin *obj)
    {
        return obj->connect();
    }

    DLL_EXPORT void Read(UnityHidApiPlugin *obj, DataRecievedCallback data_recieved, EventCallback event_callback)
    {
        obj->read(
            [data_recieved](const uint8_t *data)
            { data_recieved(data); },
            [event_callback](std::string error)
            { event_callback(error); });
    }

    DLL_EXPORT bool Disconnect(UnityHidApiPlugin *obj)
    {
        return obj->disconnect();
    }

    DLL_EXPORT bool IsConnected(UnityHidApiPlugin *obj)
    {
        return obj->isConnected();
    }

    DLL_EXPORT bool IsReading(UnityHidApiPlugin *obj)
    {
        return obj->isReading();
    }

    // Use to test if Unity is correctly loading the plugin
    DLL_EXPORT int PluginLoaded()
    {
        return 1;
    }
}