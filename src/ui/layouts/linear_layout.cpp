/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "linear_layout.h"
namespace game { namespace ui
{
  void Linear_Layout::layout_(Volume<int> vol)
  {
    // Record the data that we will need to change.
    int Vec<int>::* comp = nullptr;
    int Volume<int>::* extent = nullptr;

    // If horizontally aligning objects.
    if(orientation == Orient::Horizontal)
    {
      //.---------. <= Cell extent, component, and extent.
      //######################
      //#         #          #
      //######################

      comp = &Vec<int>::x;
      extent = &Volume<int>::width;
    }
    // If vertically aligning objects.
    else if(orientation == Orient::Vertical)
    {
      //###########-,
      //#         # | <= Cell extent, component, and extent.
      //###########-*
      //#         #
      //###########
      comp = &Vec<int>::y;
      extent = &Volume<int>::height;
    }
    else { /* I'm retarded? */ }

    // The cell extent is the length of the axis that is going to be needing
    // an adjustment.
    // For example, if the orientation is vertical, the height is going to need
    // to change, while the width will stay constant for each child's cell.
    int cell_extent = vol.*extent / children_.size();

    for(int child_i = 0; child_i < children_.size(); ++child_i)
    {
      auto view_vol = vol;

      view_vol.*extent = cell_extent;
      view_vol.pos.*comp += cell_extent * child_i;

      auto& child = children_[child_i];
      child.view->layout(view_vol);
    }
  }
  Vec<int> Linear_Layout::get_minimum_extents() const noexcept
  {
    auto bounds = Vec<int>{};

    for(auto const& child : children_)
    {
      auto const& view_vec = child.view->get_minimum_extents();

      if(orientation == Orient::Vertical)
      {
        bounds.x = std::max(bounds.x, view_vec.x);
        bounds.y += view_vec.y;
      }
      else
      {
        bounds.x += view_vec.x;
        bounds.y += std::max(bounds.y, view_vec.y);
      }
    }

    return bounds;
  }
} }
