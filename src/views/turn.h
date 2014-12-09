/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>

#include "SDL.h"

#include "../Graphics_Desc.h"
#include "../Map.h"
#include "../Sprite_Container.h"

#include "../decl/char.h"

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

  struct Uncrate_Data;

  using Turn_State = boost::variant<Waiting_Data, Moving_Data,
                                    boost::recursive_wrapper<Uncrate_Data> >;

  struct Uncrate_Data
  {
    Chest& chest;

    Turn_State after_state;

    int anim_frame = 0;
    int intermediate_counter = 0;

  };


  struct Turn_Data
  {
    Turn_Data(std::string const& items_file,
              std::string const& zones_file,
              std::string const& char_file);


    Item_Parser items;
    std::shared_ptr<Map> map;

    pong::Label<std::string> zone_label;

    player_id player;
    Turn_State state;

    // map coordinates.
    Vec<int> map_corner;

    Character_Parser character;

    void update_zone() noexcept;
    void next_player() noexcept;

    bool render_active_player_path = true;
  };

  void handle_event_state(Turn_Data&, SDL_Event const&) noexcept;
  void step_state(Turn_Data&) noexcept;
  void render_state(Graphics_Desc&, Sprite_Container&, Turn_Data&) noexcept;
}
