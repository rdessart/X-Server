cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
project("Networking" VERSION 1.0.0 LANGUAGES C CXX)

include_directories(../X-Server.Logging/include ~/vcpkg/installed/arm64-osx/include/)

add_library(Networking 
    src/NetworkUtils.cpp 
    src/UDPBeacon.cpp 
    src/UDPServer.cpp 
    include/IServer.h 
    include/UDPServer.h 
    include/Message.h 
    include/NetworkUtils.h 
    include/UDPBeacon.h
)

set_target_properties(Networking
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib/"
    LIBRARY_OUTPUT_DIRECTORY "../lib/"
    RUNTIME_OUTPUT_DIRECTORY "../lib/"
)
