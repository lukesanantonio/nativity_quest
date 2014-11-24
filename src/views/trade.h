/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"

#include "../Graphics_Desc.h"
namespace game
{
  struct Trade_Data
  {
  };

  void handle_event_state(Trade_Data&, SDL_Event const&) noexcept;
  void step_state(Trade_Data&) noexcept;
  void render_state(Graphics_Desc&, Trade_Data&) noexcept;
}

