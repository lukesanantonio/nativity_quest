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

    Vec<int> get_minimum_extents() const noexcept override;

    inline void cur(int) noexcept;
    inline int cur() const noexcept;

    inline void max(int) noexcept;
    inline int max() const noexcept;
  private:
    int cur_;
    int max_;

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
} }
