/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include "SDL.h"

#include "pong/common/vector.h"
#include "decl/items.h"

namespace game
{
  struct Player
  {
    // Position on the map.
    pong::math::vector<int> pos;
    pong::math::vector<int> delta;

    std::array<Item, 6> inventory;
  };
}
