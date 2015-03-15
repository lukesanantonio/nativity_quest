
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

include_directories(${CMAKE_SOURCE_DIR}/rapidjson)

add_subdirectory(assets)
add_subdirectory(common)
add_subdirectory(decl)
add_subdirectory(game)
add_subdirectory(ui)
add_subdirectory(states)

add_executable(quest main.cpp Map.cpp Player.cpp Entity_Health_Mediator.cpp)
target_link_libraries(quest commonlib decllib gamelib uilib viewlib assetslib)

target_link_libraries(quest ${SDL2_LIBRARIES})
target_link_libraries(quest ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(quest ${FREETYPE_LIBRARIES})
target_link_libraries(quest ${Boost_FILESYSTEM_LIBRARY}
                            ${Boost_SYSTEM_LIBRARY})
