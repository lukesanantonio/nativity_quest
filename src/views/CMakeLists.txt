
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(viewlib turn.cpp)

target_link_libraries(viewlib decllib utillib)
target_link_libraries(viewlib ${SDL2_LIBRARIES})
