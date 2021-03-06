/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \file utility.h
 * \brief Client-side utilities. SDL stuff, etc.
 *
 * Anything client specific that doesn't have a home.
 */
#pragma once
#include "SDL.h"
#include "../common/volume.h"
namespace pong
{
  inline bool operator==(const SDL_Color c1, const SDL_Color c2) noexcept
  {
    return c1.r == c2.r and c1.g == c2.g and c1.b == c2.b and c1.a == c2.a;
  }

  void render(SDL_Renderer* renderer, const Volume& volume) noexcept;
  void render_sides(SDL_Renderer*, Volume const&,
                    VolumeSides, double) noexcept;
  void render_wireframe(SDL_Renderer*, Volume const&) noexcept;
}
