/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "asset.h"
namespace game { namespace assets
{
  SDL_Texture* Image_Asset::texture(SDL_Renderer* r) noexcept
  {
    // If we are asked for a texture based on a new renderer.
    if(r != renderer_)
    {
      // Create one on the spot!
      texture_ = SDL_CreateTextureFromSurface(r, image);

      // Record the renderer that is relevant
      renderer_ = r;
    }
    // Return it!
    return texture_;
  }
} }
