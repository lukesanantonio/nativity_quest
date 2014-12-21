/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "View.h"
#include <algorithm>
namespace game { namespace ui
{
  Label& View::get_unused_label(std::string const& str) const noexcept
  {
    // Find a label using the string.
    using std::begin; using std::end;
    auto label_find = std::find_if(begin(label_cache_), end(label_cache_),
    [&str](auto const& label)
    {
      // If the label is unused and it already contains the string we can use
      // it!
      return std::get<0>(label).str == str && std::get<1>(label) == false;
    });

    if(label_find == end(label_cache_))
    {
      // Just make another
      label_cache_.push_back({Label{str}, true});
      return std::get<0>(label_cache_.back());
    }
    // Use the one we found.
    else return std::get<0>(*label_find);
  }
  void View::label(Label const& label) noexcept
  {
    auto& unused = get_unused_label(label.str);
    unused = label;
  }
  void View::box(Box const& box) noexcept
  {
    box_cache_.push_back(box);
  }
  Vec<int> View::text_size(std::string const& str) const noexcept
  {
    return {10, 10};
  }

  void View::render() const noexcept
  {
    for(auto& label_pair : label_cache_)
    {
      if(!std::get<1>(label_pair))
      {
        // This label is not being used, so rendering it is meaningless.
        continue;
      }
      auto& label = std::get<0>(label_pair);
      std::get<1>(label_pair) = false;

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

    box_cache_.clear();
  }
} }
