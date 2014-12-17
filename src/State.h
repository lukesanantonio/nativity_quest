/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>

#include "Player.h"

#include "views/turn.h"

#include <boost/variant.hpp>
namespace game
{
  enum class View
  {
    Turn
  };

  struct State
  {
    bool running = false;

    View type;
    boost::variant<game::Turn_Data> state_data;
  };
}
