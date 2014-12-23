/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "pong/common/Cache.h"

#include "common/surface.h"

#include <boost/variant.hpp>
namespace game
{
  struct Sprite_Impl
  {
    Sprite_Impl(SDL_Surface* srf = nullptr) noexcept;
    Sprite_Impl(int, std::string const&, std::string const&) noexcept;

    int id;
    std::string str;

    void reset(SDL_Surface* srf) noexcept;

    SDL_Surface* surface() const noexcept;
    SDL_Texture* texture(SDL_Renderer*) const noexcept;
  private:
    void init_cache() noexcept;

    mutable pong::Cache_With_Deleter<SDL_Texture, Texture_Deleter,
                                     Shared_Surface_Ptr, SDL_Renderer*> img_;
  };
  using Sprite = std::shared_ptr<const Sprite_Impl>;

  using sprite_id = boost::variant<int, std::string>;
  struct Unknown_Sprite_Error
  {
    sprite_id id;
  };

  // Mo' usage of the flyweight pattern for sprite management, that is,
  // everything is loaded on construction, then the constant pointers are
  // passed around.
  struct Sprite_Container
  {
    Sprite_Container(std::string const& filename);

    Sprite get_sprite(sprite_id id, int frame = 0) const noexcept;
  private:
    std::vector<std::vector<Sprite> > sprites_;
  };
}
