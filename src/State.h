/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>

#include "Player.h"

#include "views/menu.h"
#include "views/trade.h"
#include "views/turn.h"

#include <boost/variant.hpp>
namespace game
{
  enum class View
  {
    Menu,
    Turn,
    Trade
  };

  struct State
  {
    State() noexcept;

    bool running = false;

    View type;
    boost::variant<Menu_Data,
                   Turn_Data,
                   Trade_Data> state_data;
  };
}
