
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(decllib STATIC combat.cpp zones.cpp players.cpp enemies.cpp
                           effects.cpp game.cpp)

target_link_libraries(decllib commonlib)
