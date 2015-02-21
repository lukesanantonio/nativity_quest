/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "label.h"
namespace game { namespace ui
{
  Vec<int> Label::get_minimum_extents() const noexcept
  {
    gen_cache_();

    auto vec = Vec<int>{};
    SDL_QueryTexture(texture_cache_.get(), NULL, NULL, &vec.x, &vec.y);
    return vec;
  }
  Volume<int> Label::layout_()
  {
    return center_volume(parent_volume(), get_minimum_extents());
  }
  void Label::render_() const noexcept
  {
    gen_cache_();
    // If it failed it probably just means we were given an empty string or
    // something.
    if(texture_cache_)
    {
      auto dest = to_sdl_rect(this_volume());
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
