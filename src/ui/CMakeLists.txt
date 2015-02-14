
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(uilib views/label.cpp views/sprite.cpp views/bar.cpp
                  layouts/linear_layout.cpp layouts/side_layout.cpp
                  layouts/grid_layout.cpp load.cpp
                  triggers/Mouse_Click_Trigger.cpp)

target_link_libraries(uilib ${SDL2_LIBRARIES})
target_link_libraries(uilib commonlib gamelib)
