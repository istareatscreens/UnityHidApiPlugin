#include "PluginWrapper.h"
#include "UnityHidApiPlugin.h"

static std::vector<PluginPtr> &getPluginInstances()
{
    static std::vector<PluginPtr> pluginInstances;
    return pluginInstances;
}

extern "C"
{
    DLL_EXPORT UnityHidApiPlugin *Initialize(
        int vendor_id,
        int product_id,
        int buffer_size,
        int left_bytes_to_truncate,
        int bytes_to_read)
    {
        // Create new instance and store it in the smart pointer array
        PluginPtr newPlugin = std::make_unique<UnityHidApiPlugin>(
            vendor_id,
            product_id,
            buffer_size,
            left_bytes_to_truncate,
            bytes_to_read);

        UnityHidApiPlugin *pluginRawPtr = newPlugin.get();
        getPluginInstances().push_back(std::move(newPlugin));

        return pluginRawPtr;
    }

    DLL_EXPORT void Dispose(UnityHidApiPlugin *obj)
    {
        if (!obj)
        {
            return;
        }

        auto &pluginInstances = getPluginInstances();
        auto it = std::remove_if(pluginInstances.begin(), pluginInstances.end(),
                                 [obj](const PluginPtr &plugin)
                                 {
                                     return plugin.get() == obj;
                                 });

        if (it != pluginInstances.end())
        {
            pluginInstances.erase(it, pluginInstances.end());
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

    DLL_EXPORT void Read(UnityHidApiPlugin *obj, DataRecievedCallback data_recieved, EventCallback event_callback)
    {
        if (!obj)
        {
            return;
        }

        obj->read(
            [data_recieved](const uint8_t *data)
            { data_recieved(data); },
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