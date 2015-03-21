
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

cmake_minimum_required(VERSION 3.0)
project(Tommy_Game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

include(data_target)

include_directories(${CMAKE_SOURCE_DIR}/catch)
include_directories(${CMAKE_SOURCE_DIR}/ponglib)
include_directories(${CMAKE_SOURCE_DIR}/jsoncpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

find_package(Boost 1.50.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(LibUV)
include_directories(${LIBUV_INCLUDE_DIRS})

add_subdirectory(assets)

add_subdirectory(jsoncpp)
add_subdirectory(ponglib)

add_subdirectory(src)

add_subdirectory(test)
