#include "PluginWrapper.h"
#include "UnityHidApiPlugin.h"
#include <mutex>

static std::vector<UnityHidApiPlugin *> &getPluginInstances()
{
    static std::vector<UnityHidApiPlugin *> pluginInstances;
    return pluginInstances;
}

// Using a map was causing runtime crashes so switched to array
static std::vector<int> &getDeviceClassifications()
{
    static std::vector<int> deviceClassifications;
    return deviceClassifications;
}

extern "C"
{
    DLL_EXPORT UnityHidApiPlugin *Initialize(
        int device_classification,
        int vendor_id,
        int product_id,
        int buffer_size,
        int left_bytes_to_truncate,
        int bytes_to_read,
        int polling_rate_ms)
    {
        auto &pluginInstances = getPluginInstances();
        auto &deviceClassifications = getDeviceClassifications();

        for (size_t i = 0; i < deviceClassifications.size(); i++)
        {
            if (deviceClassifications[i] != device_classification)
            {
                continue;
            }
            UnityHidApiPlugin *plugin = pluginInstances[i];
            if (!plugin)
            {
                break;
            }
            plugin->stopReading();
            return plugin;
        }

        // Create new instance and store it in the smart pointer array
        auto newPlugin = new UnityHidApiPlugin(
            vendor_id,
            product_id,
            buffer_size,
            left_bytes_to_truncate,
            bytes_to_read,
            polling_rate_ms

        );

        getPluginInstances().push_back(newPlugin);
        getDeviceClassifications().push_back(device_classification);

        return newPlugin;
    }

    DLL_EXPORT void Dispose(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return;
        }

        auto &pluginInstances = getPluginInstances();
        auto &deviceClassifications = getDeviceClassifications();

        // Iterate over both pluginInstances and deviceClassifications simultaneously
        for (size_t i = 0; i < pluginInstances.size(); ++i)
        {
            if (pluginInstances[i] != obj)
            {
                continue;
            }
            pluginInstances.erase(pluginInstances.begin() + i);
            deviceClassifications.erase(deviceClassifications.begin() + i);
            return;
        }
    }

    DLL_EXPORT bool Connect(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return false;
        }
        return obj->connect();
    }

    DLL_EXPORT void Read(UnityHidApiPlugin *obj, DataReceivedCallback data_received, EventCallback event_callback)
    {
        if (!obj)
        {
            return;
        }

        obj->read(
            [data_received](const uint8_t *data)
            { data_received(data); },
            [event_callback](std::string error)
            { event_callback(error); });
    }

    DLL_EXPORT bool Disconnect(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return false;
        }

        return obj->disconnect();
    }

    DLL_EXPORT bool IsConnected(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return false;
        }

        return obj->isConnected();
    }

    DLL_EXPORT bool IsReading(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return false;
        }

        return obj->isReading();
    }

    DLL_EXPORT void StopReading(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return;
        }
        obj->stopReading();
    }

    // Use to test if Unity is correctly loading the plugin
    DLL_EXPORT int PluginLoaded()
    {
        return 1;
    }

    DLL_EXPORT void ReloadPlugin()
    {
        getPluginInstances().clear();
    }
}