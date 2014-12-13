/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Vanilla_Control.h"
namespace game
{
  void Vanilla_Control::layout(Label_View<Vanilla_Control>& view) noexcept
  {
    auto vol = view.vol();

    auto padding = Vec<int>{10, 10};
    auto cur_pos = padding;

    int i = 0;
    for(auto& label : view.labels())
    {
      label.position({cur_pos.x + vol.pos.x, cur_pos.y + vol.pos.y});

      cur_pos.x += vol.width / view.labels().size() / 2;

      if(i == view.labels().size() / 2 - 1)
      {
        cur_pos.y += vol.height / 2;
        cur_pos.x = padding.x;
      }

      ++i;
    }
  }
}
