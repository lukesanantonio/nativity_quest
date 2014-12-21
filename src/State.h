/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>

#include "Player.h"

#include "views/menu.h"
#include "views/turn.h"

#include <boost/variant.hpp>
namespace game
{
  enum class View
  {
    Turn, Menu
  };

  struct State
  {
    bool running = false;

    Vec<int> window_extents;

    View type;
    boost::variant<game::Menu_Data, game::Turn_Data> state_data;
  };
}
