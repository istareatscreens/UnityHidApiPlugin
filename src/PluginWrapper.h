/*
  Need to wrap the UnityHidApiPlugin class functions with extern functions
  to be able to interact with it in unity
  reference: https://stackoverflow.com/a/36574077
*/
#include "UnityHidApiPlugin.h"

// Declare callback
typedef int(__stdcall *DataRecievedCallback)(const uint8_t *);
typedef int(__stdcall *EventCallback)(std::string);

#ifdef __PLUGIN_WRAPPER_H
extern "C"
{
#endif

    // instantiation
    extern __declspec(dllexport) UnityHidApiPlugin *Initialize(
        int vender_id,
        int product_id,
        int buffer_size

    );

    // Need to passed intantiated object returned from Initialize to execute these function
    extern __declspec(dllexport) void Dispose(UnityHidApiPlugin *obj);
    extern __declspec(dllexport) bool Connect(UnityHidApiPlugin *obj);
    extern __declspec(dllexport) void Read(UnityHidApiPlugin *obj,
                                           DataRecievedCallback data_recieved,
                                           EventCallback event_callback);
    extern __declspec(dllexport) bool Disconnect(UnityHidApiPlugin *obj);
    extern __declspec(dllexport) bool IsConnected(UnityHidApiPlugin *obj);
    extern __declspec(dllexport) bool IsReading(UnityHidApiPlugin *obj);

#ifdef __PLUGIN_WRAPPER_H
}
#endif
