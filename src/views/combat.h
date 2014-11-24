/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"

#include "../Graphics_Desc.h"
namespace game
{
  struct Combat_Data
  {
  };

  void handle_event_state(Combat_Data&, SDL_Event const&) noexcept;
  void step_state(Combat_Data&) noexcept;
  void render_state(Graphics_Desc&, Combat_Data&) noexcept;
}

