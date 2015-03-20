
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(assetslib discovery.cpp load.cpp desc/game.cpp desc/lang.cpp
                      desc/items.cpp)

target_link_libraries(assetslib ${SDL2_LIBRARIES})
