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
    Vec<double> pos = {};

    std::array<decl::Item, 6> inventory;

    double view_radius = 85.0;
    decl::Sprite_Impl fog = {};

    Entity_Data entity_data = {0, 0, 0};

    double moved = 0.0;
    double max_movement = 0.0;

    Vec<int> spawn_pt = {};

    short sprite_frame = 0;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&) noexcept;
}
