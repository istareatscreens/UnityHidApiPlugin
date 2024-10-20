cmake_minimum_required(VERSION 3.22.2)

set(This AdxIO4ButtonRing)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# libraries
set(HIDAPI_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Library/hidapi)
set(HIDAPI_SOURCES
    ${HIDAPI_LIB_DIR}/windows/hid.c
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Library/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
add_subdirectory(${HIDAPI_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/hidapi)

# Add Dependencies
set(Headers
    ${CMAKE_SOURCE_DIR}/src/AdxIO4ButtonRing.h
    ${CMAKE_SOURCE_DIR}/src/ConnectionProperties.h
    ${HIDAPI_LIB_DIR}/hidapi/hidapi.h
)
set(Sources ${CMAKE_SOURCE_DIR}/src/AdxIO4ButtonRing.cpp)
add_library(${This} STATIC ${Sources} ${Headers} ${HIDAPI_SOURCES})

# Link hidapi
target_include_directories(${This} PRIVATE ${HIDAPI_LIB_DIR}/hidapi)
target_include_directories(${This} PUBLIC ${CMAKE_SOURCE_DIR}/src)

if(WIN32)
    target_link_libraries(${This} PRIVATE setupapi hid)
endif()

set_target_properties(${This} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

# Build
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../../Runtime/Plugins")
set_target_properties(${This} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

add_custom_command(TARGET ${This} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove "${OUTPUT_DIR}/${This}.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${This}>" "${OUTPUT_DIR}/${This}.dll"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${OUTPUT_DIR}/Debug"
    COMMENT "Copying AdxHIDButtonRing.dll to Project Plugins directory and replacing the existing file and removing Debug"
)

# Testing
include(Dart)
enable_testing()
add_subdirectory(test)