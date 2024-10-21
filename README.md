# Unity HID plugin

This plugin allows connection to hid devices in unity. It is a C++ library that implements [hidapi](https://github.com/libusb/hidapi) to interface with a HID device. Please note this only supports windows but it could be extended to support other OS.

## Compiling

Pull this into your Unity project in a directory named with a prefixed with "." or ending with a "~" to prevent unity tracking. Create a Plugins folder in your Runtime (if unity plugin) or Assets folder and adjust this code in the /CMakeLists.txt to copy the .dll library to it:

```cmake
# Copy library to unity and clean it up
add_custom_command(TARGET ${This} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove "${OUTPUT_DIR}/${This}.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${This}>" "${OUTPUT_DIR}/${This}.dll"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_DIR}/Debug"
    COMMENT "Copying AdxHIDButtonRing.dll to Project Plugins directory and replacing the existing file and removing Debug"
)
```
Make sure to pull the external dependencies before building:

```shell
git pull --recurse-submodules
```

## Usage

WIP
