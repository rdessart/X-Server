cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
project("XServer" VERSION 1.0.0 LANGUAGES C CXX)

# target_link_libraries(TimedProcessing "-framework XPLM")
# set_target_properties(TimedProcessing PROPERTIES LINK_FLAGS "-Wl,-F ${CMAKE_SOURCE_DIR}/SDK/Libraries/Mac/ -fPIC -fvisibility=hidden -arch ${ARCH}")
# if(APPLE)
#     set_target_properties(TimedProcessing PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/mac_x64)
# elseif(WIN32)
#     set_target_properties(TimedProcessing PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win_x64)
# elseif(UNIX)
# set_target_properties(TimedProcessing PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lin_x64)
# endif()
# set_target_properties(TimedProcessing PROPERTIES OUTPUT_NAME "timedProcessing")
# set_target_properties(TimedProcessing PROPERTIES PREFIX "")
# set_target_properties(TimedProcessing PROPERTIES SUFFIX ".xpl")



include_directories(
    ../X-Server.Logging/include 
    ../X-Server.Networking/include
    ../X-Server.Manager/include
    ../SDK/CHeaders/
    ~/vcpkg/installed/arm64-osx/include/)

add_definitions(-DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1 -DXPLM303=1 -DXPLM400=1 -DAPL=1)

add_library(XServer SHARED Sources/main.cpp)

# link_directories("../lib/")

target_link_libraries(
    XServer
    "-framework XPLM"
    "Networking"
    "Manager"
    "Logger")

get_test_property(XServer WORKING_DIRECTORY test_dir)
message("My test's working directory: ${test_dir}")

set_target_properties(XServer PROPERTIES LINK_FLAGS "-Wl,-F ../SDK/Libraries/Mac/ -fPIC -fvisibility=hidden")
set_target_properties(XServer PROPERTIES OUTPUT_NAME "XServer")
set_target_properties(XServer PROPERTIES PREFIX "")
set_target_properties(XServer PROPERTIES SUFFIX ".xpl")

if(APPLE)
    set_target_properties(XServer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "../Resources/plugins/XServer/mac_x64/"
        LIBRARY_OUTPUT_DIRECTORY "../Resources/plugins/XServer/mac_x64/"
        RUNTIME_OUTPUT_DIRECTORY "../Resources/plugins/XServer/mac_x64/"
    )
elseif(WIN32)
    set_target_properties(XServer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "../Resources/plugins/XServer/win_x64/"
        LIBRARY_OUTPUT_DIRECTORY "../Resources/plugins/XServer/win_x64/"
        RUNTIME_OUTPUT_DIRECTORY "../Resources/plugins/XServer/win_x64/"
    )
endif()
