/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "View.h"
#include <algorithm>
namespace game { namespace ui
{
  void View::label(Label const& label) noexcept
  {
    label_cache_.push_back(label);
  }
  void View::box(Box const& box) noexcept
  {
    box_cache_.push_back(box);
  }
  Vec<int> View::text_size(std::string const& str) const noexcept
  {
    return {10, 10};
  }

  void View::reset() noexcept
  {
    label_cache_.clear();
    box_cache_.clear();
  }
  void View::render() const noexcept
  {
    for(auto& label : label_cache_)
    {
      // TODO: Make this actually render the label
      set_render_draw_color(g_->renderer, label.color);

      SDL_Rect rect;
      rect.x = label.pos.x;
      rect.y = label.pos.y;
      rect.w = 10;
      rect.h = 10;

      SDL_RenderFillRect(g_->renderer, &rect);
    }
    for(auto const& box : box_cache_)
    {
      set_render_draw_color(g_->renderer, box.color);

      SDL_Rect rect;
      rect.x = box.vol.pos.x;
      rect.y = box.vol.pos.y;
      rect.w = box.vol.width;
      rect.h = box.vol.height;

      SDL_RenderFillRect(g_->renderer, &rect);
    }
  }
} }
