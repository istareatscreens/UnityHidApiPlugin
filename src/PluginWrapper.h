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

#include <vector>
#include <memory>
#include "UnityHidApiPlugin.h"

// Declare callback
typedef int(__stdcall *DataReceivedCallback)(const uint8_t *);
typedef int(__stdcall *EventCallback)(std::string);

typedef std::unique_ptr<UnityHidApiPlugin> PluginPtr;

extern "C"
{

  // instantiation
  extern DLL_EXPORT UnityHidApiPlugin *Initialize(
      int device_classification,
      int vendor_id,
      int product_id,
      int buffer_size,
      int left_bytes_to_truncate,
      int bytes_to_read

  );

  // Need to passed intantiated object returned from Initialize to execute these function
  extern DLL_EXPORT void Dispose(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool Connect(UnityHidApiPlugin *obj);
  extern DLL_EXPORT void Read(UnityHidApiPlugin *obj,
                              DataReceivedCallback data_received,
                              EventCallback event_callback);
  extern DLL_EXPORT bool Disconnect(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool IsConnected(UnityHidApiPlugin *obj);
  extern DLL_EXPORT bool IsReading(UnityHidApiPlugin *obj);
  extern DLL_EXPORT void StopReading(UnityHidApiPlugin *obj);
  extern DLL_EXPORT int PluginLoaded();
  extern DLL_EXPORT void ReloadPlugin();
}

#endif // __PLUGIN_WRAPPER_H
