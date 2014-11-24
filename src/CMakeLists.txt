
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

include_directories(${CMAKE_SOURCE_DIR}/rapidjson)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

find_package(Boost 1.50.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(decl)
add_subdirectory(util)
add_subdirectory(views)

add_executable(game main.cpp Map.cpp State.cpp player.cpp step.cpp
                    Graphics_Desc.cpp render.cpp)
target_link_libraries(game decllib utillib viewlib)

target_link_libraries(game ${SDL2_LIBRARIES})
target_link_libraries(game ${SDL2_IMAGE_LIBRARIES})
