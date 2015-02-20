/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "vec.h"
#include "volume.h"
#include "Color.h"
namespace game
{
  void draw_point(SDL_Renderer* r, Vec<int> p, Color c) noexcept;
  void fill_volume(SDL_Renderer* r, Volume<int> v, Color c) noexcept;
  void draw_volume(SDL_Renderer* r, Volume<int> v, Color c) noexcept;
}
