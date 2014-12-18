/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include "SDL.h"

#include "vec.h"
#include "decl/items.h"

#include "Sprite_Container.h"

namespace game
{
  struct Player
  {
    // Position on the map.
    Vec<double> pos;

    std::array<Item, 6> inventory;

    double view_radius = 100.0;
    Sprite_Impl fog;

    int life = 10;
    int max_life = 10;

    double moved;
    double max_movement;

    bool done = false;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&) noexcept;
}
