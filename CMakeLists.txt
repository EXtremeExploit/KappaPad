cmake_minimum_required(VERSION 3.12)

# Pull in Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(kappapad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Init the SDK
pico_sdk_init()

add_executable(${PROJECT_NAME}
    KappaPad/KappaPad.ino
)

target_link_libraries(${PROJECT_NAME}
pico_stdlib
)

pico_add_extra_outputs(${PROJECT_NAME})
