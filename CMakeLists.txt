
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

cmake_minimum_required(VERSION 3.0)
project(Tommy_Game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

include(data_target)

add_subdirectory(assets)
add_subdirectory(src)
