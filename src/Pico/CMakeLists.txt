cmake_minimum_required(VERSION 3.13)


set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(external/pico-sdk/pico_sdk_import.cmake)

project(pico-usb C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(main
src/main.cpp
src/usb_descriptors.cpp
src/Keyboard.cpp
src/CapacitiveKey.cpp
src/CapacitiveSensor.cpp
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)

target_link_libraries(main pico_stdlib tinyusb_device tinyusb_board)

target_include_directories(main PRIVATE src)
