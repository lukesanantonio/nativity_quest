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
                 : surface_(make_shared_surface(IMG_Load(filename.c_str())))
  {
    if(!surface_) throw Bad_File{filename};

    SDL_SetColorKey(surface_.get(), SDL_TRUE,
                    SDL_MapRGB(surface_->format, 0xff, 0x00, 0xff));

    texture_.gen_func([](auto ptr, auto r, auto surf){
      if(ptr) return ptr;
      return Texture_Ptr{SDL_CreateTextureFromSurface(r, surf)};
    });
    texture_.set_dependency<1>(surface_.get());
  }
  SDL_Surface* Sprite::surface() const noexcept
  {
    return surface_.get();
  }
  SDL_Texture* Sprite::texture(SDL_Renderer* r) const noexcept
  {
    texture_.set_dependency<0>(r);
    return texture_.cache();
  }
}
