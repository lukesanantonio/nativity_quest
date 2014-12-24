/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "pong/common/Cache.h"

#include "../common/surface.h"
namespace game { namespace decl
{
  struct Sprite_Impl
  {
    Sprite_Impl(SDL_Surface* srf = nullptr) noexcept;
    Sprite_Impl(std::string const&, std::string const&) noexcept;

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

  struct Unknown_Sprite_Error
  {
    std::string id;
  };

  struct Sprites
  {
    Sprites(std::string const& json);

    Sprite get_sprite(std::string id, int frame = 0) const;
  private:
    std::vector<std::vector<Sprite> > sprites_;
  };
} }
