cmake_minimum_required(VERSION 3.11)

project(gyro-recorder CXX)

# This is so SDL2 shared objects are placed together with our binary
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(third_party/SDL2)

set(SRC
    src/main.cpp
)

add_executable(gyro-recorder ${SRC})
target_include_directories(gyro-recorder PRIVATE third_party/SDL2/include)
target_link_libraries(gyro-recorder PUBLIC SDL2 SDL2main)