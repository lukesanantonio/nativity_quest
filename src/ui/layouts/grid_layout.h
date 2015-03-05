/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../View_Container.h"

#include <boost/optional.hpp>
namespace game { namespace ui
{
  struct Grid_Layout_Params
  {
    int row = 0;
    int col = 0;
  };

  struct Grid_Layout : public View_Container<Grid_Layout_Params>
  {
    explicit Grid_Layout(Graphics_Desc& g) noexcept : View_Container(g) {}

    inline Vec<int> get_minimum_extents() const noexcept override
    { return {0,0}; }

    bool force_fill_width = true;
    bool force_fill_height = true;

  private:
    Volume<int> layout_() override;
  };
} }
