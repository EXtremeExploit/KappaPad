cmake_minimum_required(VERSION 3.12)

# Pull in pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(kappapad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Init the SDK
pico_sdk_init()

add_executable(kappapad
    KappaPad/main.cpp
    KappaPad/Keyboard.cpp
    KappaPad/HID.cpp
    KappaPad/CapacitiveSensor.cpp
)

target_link_libraries(kappapad pico_stdlib)

pico_add_extra_outputs(kappapad)
