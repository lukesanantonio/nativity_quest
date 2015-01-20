/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View_Container.h"
namespace game { namespace ui
{
  struct Linear_Layout : public View_Container<null_layout_t>
  {
    Linear_Layout(Graphics_Desc& g) noexcept : View_Container(g) {}

    Vec<int> get_minimum_extents() const noexcept override;
    void dispatch_event(SDL_Event const&) noexcept override;

    enum class Orient
    { Vertical, Horizontal };

    Orient orientation;

  private:
    void layout_(Volume<int>) override;
    void render_() const noexcept override;
  };
} }
