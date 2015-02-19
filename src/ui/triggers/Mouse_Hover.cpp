/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Mouse_Hover.h"
#include "../View.h"
namespace game { namespace ui
{
  bool Mouse_Hover::try_trigger(View& v, SDL_Event const& event) const noexcept
  {
    if(event.type == SDL_MOUSEMOTION)
    {
      auto pt = Vec<int>{event.motion.x, event.motion.y};
      if(is_in(v.this_volume(), pt))
      {
        func_(pt);
        return true;
      }
    }
    return false;
  }
} }
