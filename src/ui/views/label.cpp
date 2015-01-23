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
  void Label::dispatch_event(SDL_Event const& event) noexcept
  {
    // We must be click able.
    if(!click_) return;

    click_trigger_.check_trigger(*this, event);
  }
  Volume<int> Label::layout_()
  {
    auto this_vol = parent_volume();

    auto spr_extents = get_minimum_extents();

    this_vol.pos.x = center(this_vol.pos.x, this_vol.width,  spr_extents.x);
    this_vol.pos.y = center(this_vol.pos.y, this_vol.height, spr_extents.y);

    this_vol.width = spr_extents.x;
    this_vol.height = spr_extents.y;

    return this_vol;
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
