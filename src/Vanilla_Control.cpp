/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Vanilla_Control.h"
namespace game
{
  void Vanilla_Control::layout(Label_View<Vanilla_Control>& view,
                               Graphics_Desc& g) noexcept
  {
    auto vol = view.vol();

    auto start = Vec<int>{vol.width / 3 / 2, 15};
    auto cur_pos = start;

    int i = 0;
    for(auto& label : view.labels())
    {
      label.position({cur_pos.x + vol.pos.x - (label.surface_extents(g).x / 2),
                      cur_pos.y + vol.pos.y});

      cur_pos.x += vol.width / 3;

      if(i == view.labels().size() / 2 - 1)
      {
        cur_pos.y += vol.height / 2;
        cur_pos.x = start.x;
      }

      ++i;
    }
  }
}
