
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(commonlib STATIC Color.cpp json.cpp render.cpp log.cpp
                             translate.cpp animation.cpp)

target_link_libraries(commonlib ${SDL2_LIBRARIES})
