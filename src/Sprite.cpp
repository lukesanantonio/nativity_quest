/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Sprite.h"

#include "SDL_image.h"
#include "util/except.h"

namespace game
{
  Sprite::Sprite(std::string const& filename)
  {
    surface_ = IMG_Load(filename.c_str());
    if(!surface_) throw Bad_File{filename};

    texture_.gen_func([](auto ptr, auto r, auto surf){
      if(ptr) return ptr;
      return Texture_Ptr{SDL_CreateTextureFromSurface(r, surf)};
    });
    texture_.set_dependency<1>(surface_);
  }
  SDL_Surface* Sprite::surface() const noexcept
  {
    return surface_;
  }
  SDL_Texture* Sprite::texture(SDL_Renderer* r) const noexcept
  {
    texture_.set_dependency<0>(r);
    return texture_.cache();
  }
}
