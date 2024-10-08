cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
project("XServerOps" VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

include_directories(
    ../X-Server.Logging/include 
    ../X-Server.Networking/include
    ../X-Server.Manager/include
    ../SDK/CHeaders/
    ~/vcpkg/installed/arm64-osx/include/)

add_definitions(-DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1 -DXPLM303=1 -DAPL=1 -DOPERATIONS_EXPORT=1)

add_library(XServerOps SHARED
    src/FlightLoopManager.h
    src/FlightLoopManager.cpp
    src/FlightLoopParameters.h
    src/DatarefManager.cpp
    src/DatarefManager.h
    src/Operations.cpp
    src/Operations.h
    src/SharedValue.h
    )

target_link_libraries(
    XServerOps
    "-framework XPLM"
    "Networking"
    "Manager"
    "Logger")
set_target_properties(XServerOps PROPERTIES COMPILE_FLAGS "-fdeclspec")
set_target_properties(XServerOps PROPERTIES LINK_FLAGS "-Wl,-F ../SDK/Libraries/Mac/ -fPIC -fvisibility=hidden -fdeclspec")

set_target_properties(XServerOps
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ".."
        LIBRARY_OUTPUT_DIRECTORY ".."
        RUNTIME_OUTPUT_DIRECTORY ".."
)