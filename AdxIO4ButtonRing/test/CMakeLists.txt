cmake_minimum_required(VERSION 3.22.2)

set(This AdxIO4ButtonRingTests)
set(Project AdxIO4ButtonRing)

set(Sources AdxIO4ButtonRingTest.cpp)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC gtest_main ${Project})
target_include_directories(${This} PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_test(NAME ${This} COMMAND ${This})