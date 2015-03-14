/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View.h"
namespace game { namespace ui
{
  struct Bar : public View
  {
    Bar(Graphics_Desc& g) noexcept : View(g) {}

    inline void cur(int) noexcept;
    inline int cur() const noexcept;

    inline void max(int) noexcept;
    inline int max() const noexcept;

    inline void color(Color const& c) noexcept;
    inline Color color() const noexcept;
  private:
    int cur_;
    int max_;

    Color color_;

    Vec<int> get_minimum_extents_() const noexcept override;
    Volume<int> layout_() override;
    void render_() const noexcept override;
  };

  inline void Bar::cur(int cur) noexcept
  {
    // TODO implement some sort of animation when rendering.
    // - A flag to enable it maybe?
    cur_ = cur;
  }
  inline int Bar::cur() const noexcept
  {
    return cur_;
  }

  inline void Bar::max(int max) noexcept
  {
    max_ = max;
  }
  inline int Bar::max() const noexcept
  {
    return max_;
  }

  inline void Bar::color(Color const& c) noexcept
  {
    color_ = c;
  }
  inline Color Bar::color() const noexcept
  {
    return color_;
  }
} }
