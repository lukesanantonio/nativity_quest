/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include "SDL.h"

#include "common/vec.h"
#include "decl/items.h"
#include "decl/sprites.h"

#include "Entity_Data.h"

namespace game
{
  struct Player
  {
    // Position on the map.
    Vec<double> pos;

    std::array<decl::Item, 6> inventory;

    double view_radius = 85.0;
    decl::Sprite_Impl fog;

    Entity_Data entity_data;

    double moved;
    double max_movement;

    Vec<int> spawn_pt;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&) noexcept;
}
