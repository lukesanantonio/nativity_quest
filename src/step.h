/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "SDL.h"

namespace game
{
  struct State;
  struct Graphics_Desc;

  void handle_event(State&, SDL_Event const&) noexcept;
  void step(State&) noexcept;
  void render(State&, Graphics_Desc&) noexcept;
}