
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(gamelib STATIC Font_Renderer.cpp Game.cpp Graphics_Desc.cpp)

target_link_libraries(gamelib ${SDL2_LIBRARIES})
target_link_libraries(gamelib ${SDL2_TTF_LIBRARIES})
target_link_libraries(gamelib commonlib)
