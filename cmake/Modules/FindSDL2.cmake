# - Find SDL2 library and headers
# 
# Find module for SDL 2.0 (http://www.libsdl.org/).
# It defines the following variables:
#  SDL2_INCLUDE_DIRS - The location of the headers, e.g., SDL.h.
#  SDL2_LIBRARIES - The libraries to link against to use SDL2.
#  SDL2_FOUND - If false, do not try to use SDL2.
#  SDL2_VERSION_STRING - Human-readable string containing the version of SDL2.
#
# Also defined, but not for general use are:
#   SDL2_INCLUDE_DIR - The directory that contains SDL.h.
#   SDL2_LIBRARY - The location of the SDL2 library.
#
# Copyright 2013 Benjamin Eikel
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

find_package(PkgConfig QUIET)
pkg_check_modules(PC_SDL2 QUIET sdl2)

find_path(SDL2_INCLUDE_DIR
  NAMES SDL.h
  HINTS
    ${PC_SDL2_INCLUDEDIR}
    ${PC_SDL2_INCLUDE_DIRS}
  PATH_SUFFIXES SDL2
)

find_library(SDL2_LIBRARY
  NAMES SDL2
  HINTS
    ${PC_SDL2_LIBDIR}
    ${PC_SDL2_LIBRARY_DIRS}
  PATH_SUFFIXES x64 x86
)

if(SDL2_INCLUDE_DIR AND EXISTS "${SDL2_INCLUDE_DIR}/SDL_version.h")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+[0-9]+$")
  string(REGEX REPLACE "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MAJOR "${SDL2_VERSION_MAJOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MINOR "${SDL2_VERSION_MINOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_PATCH "${SDL2_VERSION_PATCH_LINE}")
  set(SDL2_VERSION_STRING ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})
  unset(SDL2_VERSION_MAJOR_LINE)
  unset(SDL2_VERSION_MINOR_LINE)
  unset(SDL2_VERSION_PATCH_LINE)
  unset(SDL2_VERSION_MAJOR)
  unset(SDL2_VERSION_MINOR)
  unset(SDL2_VERSION_PATCH)
endif()

set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
set(SDL2_LIBRARIES ${SDL2_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDL2
                                  REQUIRED_VARS SDL2_INCLUDE_DIR SDL2_LIBRARY
                                  VERSION_VAR SDL2_VERSION_STRING)

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)
