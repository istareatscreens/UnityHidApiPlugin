#include "PluginWrapper.h"
#include "UnityHidApiPlugin.h"

UnityHidApiPlugin *Initialize(int vender_id, int product_id, int buffer_size)
{
    return nullptr;
}

void Dispose(UnityHidApiPlugin *obj)
{
    obj->~UnityHidApiPlugin();
}

bool Connect(UnityHidApiPlugin *obj)
{
    return obj->connect();
}

void Read(UnityHidApiPlugin *obj, DataRecievedCallback data_recieved, EventCallback event_callback)
{
    obj->read(data_recieved, event_callback);
}

bool Disconnect(UnityHidApiPlugin *obj)
{
    return obj->disconnect();
}

bool IsConnected(UnityHidApiPlugin *obj)
{
    return obj->isConnected();
}

bool IsReading(UnityHidApiPlugin *obj)
{
    return obj->isReading();
}
