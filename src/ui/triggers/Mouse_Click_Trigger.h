/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../Event_Trigger.h"
#include "../View.h"
namespace game { namespace ui
{
  struct Mouse_Click_Trigger : public Event_Trigger<Vec<int> >
  {
    void check_trigger(View& v, SDL_Event const& event) const noexcept;
  };
} }
