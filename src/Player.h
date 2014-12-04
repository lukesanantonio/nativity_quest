/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include "SDL.h"

#include "vec.h"
#include "decl/items.h"

namespace game
{
  struct Player
  {
    // Position on the map.
    Vec<double> pos;

    std::array<Item, 6> inventory;
  };
}
