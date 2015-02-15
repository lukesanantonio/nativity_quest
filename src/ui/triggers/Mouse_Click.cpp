/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Mouse_Click.h"
namespace game { namespace ui
{
  bool Mouse_Click::
  try_trigger(View& v, SDL_Event const& event) const noexcept
  {
    if(event.type == SDL_MOUSEBUTTONDOWN &&
       event.button.button == SDL_BUTTON_LEFT)
    {
      auto pt = Vec<int>{event.button.x, event.button.y};
      if(is_in(v.this_volume(), pt))
      {
        func_(pt);
        return true;
      }
    }
    return false;
  }
} }
