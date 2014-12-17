/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"
#include "../volume.h"
namespace game
{
  template <typename T> auto to_rect(Volume<T> const& vol) noexcept -> SDL_Rect
  {
    auto rect = SDL_Rect{};

    rect.x = vol.pos.x;
    rect.y = vol.pos.y;
    rect.w = vol.width;
    rect.h = vol.height;

    return rect;
  }
}
