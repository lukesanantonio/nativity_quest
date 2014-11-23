/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "util/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(std::string const& map,
           std::string const& items,
           std::string const& zone_json)
           : items(items), zones(zone_json, items)
  {
    surface_ = IMG_Load(map.c_str());
    if(!surface_) throw Bad_File{map};

    texture_.gen_func([](auto ptr, auto renderer, auto surface){
      if(ptr) return ptr;
      return Texture_Ptr{SDL_CreateTextureFromSurface(renderer, surface)};
    });
    texture_.set_dependency<1>(surface_);
  }

  SDL_Surface* Map::surface() const noexcept
  {
    return surface_;
  }
  SDL_Texture* Map::texture(SDL_Renderer* renderer) const noexcept
  {
    texture_.set_dependency<0>(renderer);
    return texture_.cache();
  }
}
