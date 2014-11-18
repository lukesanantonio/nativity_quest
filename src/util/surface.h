/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>
#include "SDL.h"
namespace game
{
  struct Surface_Deleter
  {
    void operator()(SDL_Surface* surface) noexcept
    {
      SDL_FreeSurface(surface);
    }
  };

  using Surface_Ptr = std::unique_ptr<SDL_Surface, Surface_Deleter>;
}
