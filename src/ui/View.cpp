/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "View.h"
#include <algorithm>
namespace game { namespace ui
{
  Label::Label(Label const& label) noexcept
              : str{label.str}, pos{label.pos}, color(label.color) {}

  Label& Label::operator=(Label const& label) noexcept
  {
    str = label.str;
    pos = label.pos;
    color = label.color;

    surface_cache_.release();
    surface_cache_.release();

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
  Vec<int> View::text_size(std::string const& str) const noexcept
  {
    using std::begin; using std::end;
    auto label_find = std::find_if(begin(label_cache_), end(label_cache_),
    [&str](auto const& label)
    {
      return label.str == str;
    });

    if(label_find != end(label_cache_))
    {
      generate_label_surface_cache(*label_find);

      SDL_Surface* surface = label_find->surface_cache_.get();

      auto size = Vec<int>{surface->w, surface->h};
      return size;
    }
    else
    {
      SDL_Surface* surface = TTF_RenderText_Blended(g_->small_font, str.data(),
                                                    {0x00, 0x00, 0x00, 0x00});
      auto size = Vec<int>{surface->w, surface->h};

      SDL_FreeSurface(surface);

      return size;
    }
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

      if(!label.texture_cache_)
      {
        if(!label.surface_cache_)
        {
          generate_label_surface_cache(label);
        }
        label.texture_cache_.reset(
          SDL_CreateTextureFromSurface(g_->renderer,
                                       label.surface_cache_.get()));
      }

      SDL_Rect dest;
      dest.x = label.pos.x;
      dest.y = label.pos.y;
      dest.w = label.surface_cache_->w;
      dest.h = label.surface_cache_->h;

      SDL_RenderCopy(g_->renderer, label.texture_cache_.get(), NULL, &dest);
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

  void View::generate_label_surface_cache(Label const& l) const noexcept
  {
    if(l.surface_cache_) return;

    l.surface_cache_.reset(
      TTF_RenderText_Blended(g_->small_font, l.str.c_str(),
                             {l.color.red,l.color.green,l.color.blue,0xff}));
  }
} }
