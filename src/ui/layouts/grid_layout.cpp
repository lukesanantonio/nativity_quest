/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "grid_layout.h"
namespace game { namespace ui
{
  Volume<int> Grid_Layout::layout_()
  {
    auto vol = parent_volume();

    // Find the max row and column.
    int max_row = 0, max_col = 0;
    for(auto const& child : children_)
    {
      max_row = std::max(max_row, child.layout.row);
      max_col = std::max(max_col, child.layout.col);
    }

    // Measure the cells
    ++max_row; ++max_col;
    auto cell_extents = Vec<int>{vol.width / max_col, vol.height / max_row};

    for(auto const& child : children_)
    {
      auto child_vol = Volume<int>{vol.pos, cell_extents.x, cell_extents.y};
      child_vol.pos.x += child.layout.col * cell_extents.x;
      child_vol.pos.y += child.layout.row * cell_extents.y;
      child.view->layout(child_vol);
    }

    return vol;
  }
} }
