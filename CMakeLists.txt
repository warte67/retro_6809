# MINIMUM CMAKE VERSION
cmake_minimum_required(VERSION 3.22)

# SET THE PROJECT NAME
set(PROJECT_NAME main)
project(${PROJECT_NAME})

# FIND THE SDL STUFF
find_package(SDL3 REQUIRED)

# SOURCE FILES
add_executable(${PROJECT_NAME}
    ./src/main.cpp
    ./src/IDevice.cpp
    ./src/Bus.cpp
    ./src/GPU.cpp
    ./src/Memory.cpp
)

# INCLUDE DIRECTORIES
include_directories(
    ${SDL3_INCLUDE_DIR}
    ./include)

# LINK LIBRARIES
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    SDL3::SDL3
)

# COMPILER FLAGS
target_compile_options(${PROJECT_NAME} PRIVATE 
    -std=c++23
    -O2
    -fexceptions
    -Wall
    #-DDEBUG
)