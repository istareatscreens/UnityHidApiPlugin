/*
  Need to wrap the UnityHidApiPlugin class functions with extern functions
  to be able to interact with it in unity
  reference: https://stackoverflow.com/a/36574077
*/
#ifndef __PLUGIN_WRAPPER_H
#define __PLUGIN_WRAPPER_H

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include "UnityHidApiPlugin.h"

// Declare callback
typedef int(__stdcall *DataRecievedCallback)(const uint8_t *);
typedef int(__stdcall *EventCallback)(std::string);

extern "C"
{

  // instantiation
  extern DLL_EXPORT UnityHidApiPlugin *Initialize(
      int vendor_id,
      int product_id,
      int buffer_size

  );

  // Need to passed intantiated object returned from Initialize to execute these function
  extern DLL_EXPORT void Dispose(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool Connect(UnityHidApiPlugin *obj);
  extern DLL_EXPORT void Read(UnityHidApiPlugin *obj,
                              DataRecievedCallback data_recieved,
                              EventCallback event_callback);
  extern DLL_EXPORT bool Disconnect(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool IsConnected(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool IsReading(UnityHidApiPlugin *obj);
  extern DLL_EXPORT int PluginLoaded();
}

#endif // __PLUGIN_WRAPPER_H
