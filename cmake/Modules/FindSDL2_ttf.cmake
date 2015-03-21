
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

find_package(PkgConfig)
pkg_check_modules(PC_SDL2_TTF QUIET SDL2_ttf)

find_path(SDL2_TTF_INCLUDE_DIRS SDL_ttf.h
          HINTS ${PC_SDL2_TTF_INCLUDE_DIRS}
          PATH_SUFFIXES SDL2)

find_library(SDL2_TTF_LIBRARIES SDL2_ttf HINTS ${PC_SDL2_TTF_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_TTF, DEFAULT_MSG
                                  SDL2_TTF_INCLUDE_DIRS SDL2_TTF_LIBRARIES)

