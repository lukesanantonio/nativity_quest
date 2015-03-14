/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "bar.h"
#include "../../common/center.hpp"
#include "../../common/Color.h"
namespace game { namespace ui
{
  Vec<int> Bar::get_minimum_extents() const noexcept
  {
    return {0, 0};
  }
  Volume<int> Bar::layout_()
  {
    return this->parent_volume();
  }

  void Bar::render_() const noexcept
  {
    auto vol = parent_volume();

    auto rect = to_sdl_rect(parent_volume());

    set_render_draw_color(graphics_.renderer, color_);
    SDL_RenderFillRect(graphics_.renderer, &rect);

    // Reduce the parent volume by a certain amount.
    // Divide the width into equal segments.
    auto seg_size = rect.w / max_;
    vol.width -= seg_size * (max_ - cur_);
    rect = to_sdl_rect(parent_volume());
    SDL_RenderFillRect(graphics_.renderer, &rect);
  }
} }
