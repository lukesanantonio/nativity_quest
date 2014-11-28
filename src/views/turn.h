/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "SDL.h"

#include "../Graphics_Desc.h"
#include "../Map.h"
#include "../Sprite_Container.h"

#include "pong/render/widgets/Label.h"
namespace game
{
  struct Turn_Data
  {
    Turn_Data(Map map, std::string char_img) noexcept;

    Map map;
    short current_player;

    Sprite character;

    pong::Label<std::string> zone_label;
  };

  void handle_event_state(Turn_Data&, SDL_Event const&) noexcept;
  void step_state(Turn_Data&) noexcept;
  void render_state(Graphics_Desc&, Sprite_Container&, Turn_Data&) noexcept;
}
