/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View_Container.h"
namespace game { namespace ui
{
  struct Linear_Layout_Params
  {
    int weight = 1;
  };

  struct Linear_Layout : public View_Container<Linear_Layout_Params>
  {
    explicit Linear_Layout(Graphics_Desc& g) noexcept : View_Container(g) {}

    Vec<int> get_minimum_extents() const noexcept override;

    enum class Orient
    { Vertical, Horizontal };

    Orient orientation;
    bool force_fill = true;

  private:
    Volume<int> layout_() override;
  };
} }
