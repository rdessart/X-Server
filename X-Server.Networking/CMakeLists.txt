cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
project("Networking" VERSION 1.0.0 LANGUAGES C CXX)

include_directories(../X-Server.Logging/include ~/vcpkg/installed/arm64-osx/include/)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h include/*.cpp include/*.h)

add_library(Networking  ${SOURCES} )

set_target_properties(Networking
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "../lib/"
    LIBRARY_OUTPUT_DIRECTORY "../lib/"
    RUNTIME_OUTPUT_DIRECTORY "../lib/"
)
