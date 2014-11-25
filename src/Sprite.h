/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "pong/common/Cache.h"
#include "util/surface.h"

#include "SDL.h"

namespace game
{
  struct Sprite
  {
    Sprite(std::string const&);

    SDL_Surface* surface() const noexcept;
    SDL_Texture* texture(SDL_Renderer*) const noexcept;
  private:
    Shared_Surface_Ptr surface_;
    mutable pong::Cache_With_Deleter<SDL_Texture, Texture_Deleter,
                                     SDL_Renderer*, SDL_Surface*> texture_;
  };
}
