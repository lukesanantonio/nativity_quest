
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(viewlib menu.cpp navigate.cpp move.cpp uncrate.cpp discard.cpp)

target_link_libraries(viewlib ${SDL2_LIBRARIES})
target_link_libraries(viewlib decllib commonlib uilib)
