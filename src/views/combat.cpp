/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
namespace game
{
  void handle_event_state(Combat_Data&, SDL_Event const&) noexcept {}
  void step_state(Combat_Data&) noexcept {}
  void render_state(Graphics_Desc&, Sprite_Container&, Combat_Data&) noexcept {}
}
