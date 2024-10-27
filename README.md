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

## Debugging

### Debugging with tests:

Make sure the change the add_library argument to STATIC (in ./CMakeLists.txt) and comment out the POST_BUILD command (if using unity at the same time due to file lock)

You also need to change static linking of msvc runtime to debug mode. So change the following line

```
   target_compile_options(${This} PRIVATE /clr- /MT)
```

To

```
    target_compile_options(${This} PRIVATE /clr- /MD)
```

### Validating API symbols are non mangled and present

If you have problems connecting to the plugin in Unity ensure that the symbols exist, you can use dumpbin.exe Provided as tools with Visaul Studio (Typical path: `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\<Your Version>\bin\Hostx64\x64 `). For example:

```
.\dumpbin.exe /export <path to UnityHidApiPlugin.dll>
```

You should see non-mangled function names present in the output from this command. Ensure that the add_library argument in (./CMakeLists.txt) is set to SHARED.

## Usage

You can use the following C# class to wrap this plugin [HERE](https://github.com/istareatscreens/MychIO/blob/a596661cba4d541f248779030497743804ec474b/Runtime/Connection/HID/UnityHidApiPlugin.cs#L6-L75)

An example instantiation of this plugin can be found [HERE](https://github.com/istareatscreens/MychIO/blob/a596661cba4d541f248779030497743804ec474b/Runtime/Connection/HID/HidDeviceConnection.cs#L14-L185):

## Refernces

- [CMake Setup](https://www.youtube.com/watch?v=Lp1ifh9TuFI)
- [Debugging With CMake and VSCode](https://www.youtube.com/watch?v=OUAuqw3QgeE)
- [C++ Unity Plugin Documentation](https://www.mono-project.com/docs/advanced/pinvoke/)
- [hidapi Example](https://www.codeproject.com/KB/DLL/XDllPt3.aspx)
