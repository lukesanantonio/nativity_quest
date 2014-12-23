
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

add_library(uilib Model.cpp Presenter.cpp View.cpp)

target_link_libraries(uilib ${SDL2_LIBRARIES})
target_link_libraries(uilib commonlib gamelib)
