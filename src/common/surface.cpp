/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "surface.h"
namespace game
{
  SDL_Texture* Drawable_Surface::texture(SDL_Renderer* r) const noexcept
  {
    // If we are asked for a texture based on a new renderer.
    if(r != renderer_)
    {
      // Create one on the spot!
      texture_ = SDL_CreateTextureFromSurface(r, surface.get());

      // Record the renderer that is relevant
      renderer_ = r;
    }
    // Return it!
    return texture_;
  }
}
