/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "label.h"
#include "../../common/center.hpp"
namespace game { namespace ui
{
  Vec<int> Label::get_minimum_extents() const noexcept
  {
    gen_cache_();

    auto vec = Vec<int>{};
    SDL_QueryTexture(texture_cache_.get(), NULL, NULL, &vec.x, &vec.y);
    return vec;
  }
  void Label::dispatch_event(SDL_Event const&) noexcept {}
  void Label::render_() const noexcept
  {
    auto dest = to_sdl_rect(volume_());

    auto spr_extents = get_minimum_extents();

    dest.x = center(dest.x, dest.w, spr_extents.x);
    dest.y = center(dest.y, dest.h, spr_extents.y);

    dest.w = spr_extents.x;
    dest.h = spr_extents.y;

    gen_cache_();
    // If it failed it probably just means we were given an empty string or
    // something.
    if(texture_cache_)
    {
      SDL_RenderCopy(graphics_.renderer, texture_cache_.get(), NULL, &dest);
    }
  }
  void Label::gen_cache_() const noexcept
  {
    if(texture_cache_) return;
    auto surface = fr_.render_text(str_, size_, col_);
    auto texture = SDL_CreateTextureFromSurface(graphics_.renderer, surface);
    texture_cache_.reset(texture);
  }
} }
