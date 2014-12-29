
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(decllib STATIC combat.cpp items.cpp zones.cpp players.cpp
                           enemies.cpp sprites.cpp)

target_link_libraries(decllib commonlib)
