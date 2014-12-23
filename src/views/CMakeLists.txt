
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(viewlib menu.cpp)

target_link_libraries(viewlib decllib utillib uilib)
target_link_libraries(viewlib ${SDL2_LIBRARIES})
