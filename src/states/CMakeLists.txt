
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(viewlib menu.cpp navigate.cpp player_switch.cpp uncrate.cpp
            player_intro.cpp inventory.cpp discard.cpp move.cpp flare.cpp
            view_hud.cpp win.cpp combat.cpp combat_intro.cpp intro.cpp)

target_link_libraries(viewlib ${SDL2_LIBRARIES})
target_link_libraries(viewlib decllib commonlib uilib)
