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

#include <boost/variant.hpp>
namespace game
{
  struct Waiting_Data
  {
  };
  struct Moving_Data
  {
    Vec<double> delta;
  };

  using Turn_State = boost::variant<Waiting_Data, Moving_Data>;

  struct Turn_Data
  {
    Turn_Data(std::string const& items_file,
              std::string const& zones_file) noexcept;

    Item_Parser items;
    Map map;

    pong::Label<std::string> zone_label;

    player_id player;
    Turn_State state;

    // map coordinates.
    Vec<int> map_corner;

    void update_zone() noexcept;
    void next_player() noexcept;

    bool render_active_player_path = true;
  };

  void handle_event_state(Turn_Data&, SDL_Event const&) noexcept;
  void step_state(Turn_Data&) noexcept;
  void render_state(Graphics_Desc&, Sprite_Container&, Turn_Data&) noexcept;
}
