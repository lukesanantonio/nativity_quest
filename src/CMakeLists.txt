
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

include_directories(${CMAKE_SOURCE_DIR}/rapidjson)

add_subdirectory(common)
add_subdirectory(decl)
add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(views)

add_executable(quest main.cpp Map.cpp Player.cpp)
target_link_libraries(quest commonlib decllib gamelib uilib viewlib)

target_link_libraries(quest ${SDL2_LIBRARIES})
target_link_libraries(quest ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(quest ${FREETYPE_LIBRARIES})
