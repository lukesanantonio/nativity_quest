/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "View.h"
#include <algorithm>
namespace game { namespace ui
{
  Label::Label(Label const& rhs) noexcept
               : str{rhs.str},
                 size{rhs.size},
                 pos{rhs.pos},
                 color(rhs.color) {}

  Label& Label::operator=(Label const& label) noexcept
  {
    str = label.str;
    size = label.size;
    pos = label.pos;
    color = label.color;

    surface_cache_.release();
    texture_cache_.release();

    return *this;
  }
  void View::label(Label const& label) noexcept
  {
    label_cache_.push_back(label);
  }
  void View::box(Box const& box) noexcept
  {
    box_cache_.push_back(box);
  }
  Vec<int> View::text_size(std::string str, int size) const noexcept
  {
    using std::begin; using std::end;
    auto label_find = std::find_if(begin(label_cache_), end(label_cache_),
    [&](auto const& label)
    {
      return label.str == str && label.size == size;
    });

    // If we already have a label with the same data, we can generate the
    // label now and save it for later!
    if(label_find != end(label_cache_))
    {
      generate_label_surface_cache(*label_find);

      SDL_Surface* surface = label_find->surface_cache_.get();

      auto size = Vec<int>{surface->w, surface->h};
      return size;
    }
    // Well we just need to render some text and forget about it.
    else
    {
      SDL_Surface* surface = font_->render_text(str, size,
                                                {0x00, 0x00, 0x00});
      auto size = Vec<int>{surface->w, surface->h};

      SDL_FreeSurface(surface);

      return size;
    }
  }

  void View::reset() noexcept
  {
    // Clear all of the things we were going to draw.
    label_cache_.clear();
    box_cache_.clear();
  }
  void View::render(Graphics_Desc& g) const noexcept
  {
    // For each label
    for(auto& label : label_cache_)
    {
      // Generate the texture
      if(!label.texture_cache_)
      {
        // ... But first the surface if necessary.
        generate_label_surface_cache(label);
        label.texture_cache_.reset(
          SDL_CreateTextureFromSurface(g.renderer,
                                       label.surface_cache_.get()));
      }

      SDL_Rect dest;
      dest.x = label.pos.x;
      dest.y = label.pos.y;
      dest.w = label.surface_cache_->w;
      dest.h = label.surface_cache_->h;

      SDL_RenderCopy(g.renderer, label.texture_cache_.get(), NULL, &dest);
    }
    for(auto const& box : box_cache_)
    {
      set_render_draw_color(g.renderer, box.color);

      SDL_Rect rect;
      rect.x = box.vol.pos.x;
      rect.y = box.vol.pos.y;
      rect.w = box.vol.width;
      rect.h = box.vol.height;

      SDL_RenderDrawRect(g.renderer, &rect);
    }
  }

  void View::generate_label_surface_cache(Label const& l) const noexcept
  {
    if(l.surface_cache_) return;

    l.surface_cache_.reset(font_->render_text(l.str, l.size, l.color));
  }
} }
