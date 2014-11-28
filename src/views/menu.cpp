/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "menu.h"
namespace game
{
  void handle_event_state(Menu_Data&, SDL_Event const&) noexcept {}
  void step_state(Menu_Data&) noexcept {}
  void render_state(Graphics_Desc&, Sprite_Container&, Menu_Data&) noexcept {}
}
