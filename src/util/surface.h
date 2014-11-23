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

  struct Texture_Deleter
  {
    void operator()(SDL_Texture* texture) noexcept
    {
      SDL_DestroyTexture(texture);
    }
  };
  using Texture_Ptr = std::unique_ptr<SDL_Texture, Texture_Deleter>;
}
