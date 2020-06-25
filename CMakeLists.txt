cmake_minimum_required(VERSION 3.1)
project(Submarine)

# Configure CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

file(GLOB SOURCE_FILES
    src/*.c)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)


include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_image_INCLUDE_DIR})
include_directories(${SDL2_mixer_INCLUDE_DIR})
include_directories(${SDL2_ttf_INCLUDE_DIR})

add_executable(Submarine ${SOURCE_FILES})

target_link_libraries(Submarine ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})
