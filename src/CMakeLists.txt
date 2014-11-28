
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

include_directories(${CMAKE_SOURCE_DIR}/rapidjson)

add_subdirectory(decl)
add_subdirectory(util)
add_subdirectory(views)

add_executable(game main.cpp Map.cpp State.cpp Player.cpp step.cpp
                    Graphics_Desc.cpp render.cpp Sprite_Container.cpp)
target_link_libraries(game decllib utillib viewlib)

target_link_libraries(game io_core render_core)

target_link_libraries(game ${SDL2_LIBRARIES})
target_link_libraries(game ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(game ${FREETYPE_LIBRARIES})
