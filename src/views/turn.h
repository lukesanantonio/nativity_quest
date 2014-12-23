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
#include "../Inventory_View_Control.h"
#include "../Combat_Control.h"
#include "../State.h"

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

  struct Winning_Data
  {
    Managed_Label label;
  };

  struct Flare_Data;

  using Turn_State =
    boost::variant<Waiting_Data, Moving_Data,
                   boost::recursive_wrapper<Uncrate_Data>,
                   boost::recursive_wrapper<Discard_Item_Data>,
                   boost::recursive_wrapper<Inventory_View_Data>,
                   boost::recursive_wrapper<Combat_Data>,
                   Change_View_Data,
                   Winning_Data,
                   boost::recursive_wrapper<Flare_Data> >;

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
    Label_View<Inventory_View_Control> label_view;
  };

  struct Combat_Data
  {
    Turn_State after_state;

    Label_View<Combat_Control> label_view;
  };

  struct Flare_Data
  {
    int steps = 15;
  };

  struct Turn_Data : public Game_State
  {
    Turn_Data(std::string const& items_file,
              std::string const& zones_file,
              std::string const& char_file,
              std::string const& enemies_file);


    mutable Item_Parser items;
    mutable std::shared_ptr<Map> map;

    mutable Managed_Label zone_label;

    mutable player_id player;
    mutable Turn_State state;

    // map coordinates.
    mutable Vec<int> map_corner;

    mutable Character_Parser character;

    void handle_event(State&, SDL_Event const&) noexcept;
    void step(State&) noexcept override;
    void render(State const&, Graphics_Desc&,
                Sprite_Container&) const noexcept override;

    inline bool is_toplevel() const noexcept override { return false; }
  };

  Turn_State change_player(Turn_Data& turn) noexcept;
}
