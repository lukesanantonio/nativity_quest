/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>

#include "player.h"
#include "Map.h"

#include <boost/variant.hpp>
namespace game
{
  enum class View
  {
    Menu,
    Player_Turn,
    Combat,
    Trade
  };

  struct Menu_Data {};
  struct Player_Turn_Data
  {
    Map map;
    short current_player_;
  };
  struct Combat_Data {};
  struct Trade_Data {};

  struct State
  {
    State() noexcept;

    bool running = false;

    View type;
    boost::variant<Menu_Data,
                   Player_Turn_Data,
                   Combat_Data,
                   Trade_Data> state_data;
  };
}
