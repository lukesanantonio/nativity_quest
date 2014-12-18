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

#include "../Label_View.h"
#include "../Discard_Item_Control.h"
#include "../Vanilla_Control.h"
#include "../Combat_Control.h"

#include "../decl/char.h"
#include "../decl/enemies.h"

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

  struct Discard_Item_Data;

  struct Inventory_View_Data;

  struct Combat_Data;

  struct Change_View_Data
  {
    Vec<int> delta;
  };

  using Turn_State =
    boost::variant<Waiting_Data, Moving_Data,
                   boost::recursive_wrapper<Uncrate_Data>,
                   boost::recursive_wrapper<Discard_Item_Data>,
                   boost::recursive_wrapper<Inventory_View_Data>,
                   boost::recursive_wrapper<Combat_Data>,
                   Change_View_Data>;

  struct Uncrate_Data
  {
    Chest& chest;

    Turn_State after_state;

    int anim_frame = 0;
    int intermediate_counter = 0;

  };

  struct Discard_Item_Data
  {
    Discard_Item_Data(Player& p, Item_Parser const& items) noexcept
                      : label_view(Volume<int>{},
                                   Discard_Item_Control{p, items}) {}

    Label_View<Discard_Item_Control> label_view;

    Turn_State after_state;
  };

  struct Inventory_View_Data
  {
    Inventory_View_Data() noexcept : label_view() {}

    Turn_State after_state;
    Label_View<Vanilla_Control> label_view;
  };

  struct Combat_Data
  {
    Turn_State after_state;

    Label_View<Combat_Control> label_view;
  };

  struct Turn_Data
  {
    Turn_Data(std::string const& items_file,
              std::string const& zones_file,
              std::string const& char_file,
              std::string const& enemies_file);


    Item_Parser items;
    std::shared_ptr<Map> map;

    Managed_Label zone_label;

    player_id player;
    Turn_State state;

    // map coordinates.
    Vec<int> map_corner;

    Character_Parser character;

    bool render_active_player_path = false;
  };

  Turn_State change_player(Turn_Data& turn) noexcept;

  struct State;

  void handle_event_state(State& s, Turn_Data&, SDL_Event const&) noexcept;
  void step_state(State& s, Turn_Data&) noexcept;
  void render_state(State& s, Graphics_Desc&,
                    Sprite_Container&, Turn_Data&) noexcept;
}
