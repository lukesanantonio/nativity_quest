/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"

#include "../Graphics_Desc.h"
#include "../Map.h"
#include "../Sprite.h"

#include "pong/render/widgets/Label.h"
namespace game
{
  struct Turn_Data
  {
    Map map;
    short current_player;

    Sprite character;

    pong::Label<std::string> zone_label;
  };

  void initialize_state(Turn_Data&) noexcept;

  void handle_event_state(Turn_Data&, SDL_Event const&) noexcept;
  void step_state(Turn_Data&) noexcept;
  void render_state(Graphics_Desc&, Turn_Data&) noexcept;
}
