# MINIMUM CMAKE VERSION
cmake_minimum_required(VERSION 3.22)

# SET THE PROJECT NAME
set(PROJECT_NAME main)
project(${PROJECT_NAME})

# FIND THE SDL STUFF
find_package(SDL3 REQUIRED)

# GLOB ALL SOURCE FILES FROM THE ./src FOLDER
file(GLOB SRC_FILES ./src/*.cpp)

# ADD EXECUTABLE
add_executable(${PROJECT_NAME} ${SRC_FILES})

# INCLUDE DIRECTORIES
include_directories(
    ${SDL3_INCLUDE_DIR}
    ./include
)

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
