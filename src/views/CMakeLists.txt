
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(viewlib turn.cpp menu.cpp)

target_link_libraries(viewlib decllib utillib uilib)
target_link_libraries(viewlib ${SDL2_LIBRARIES})
