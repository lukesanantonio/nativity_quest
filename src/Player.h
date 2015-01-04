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
  namespace decl
  {
    struct Effects;
  }

  struct Player
  {
    // Position on the map.
    Vec<double> pos = {};

    std::array<decl::Item, 6> inventory;

    double view_radius = 65.0;
    decl::Sprite_Impl fog = {};

    Entity_Data entity_data = {0, 0, 0};

    double moved = 0.0;
    double max_movement = 75.0;

    Vec<int> spawn_pt = {};

    short sprite_frame = 0;

    // effect variables
    int turns_of_haste = 0;
    int combat_defense = 0;
    bool flare = false;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&, decl::Effects&) noexcept;

  inline void respawn(Player& p) noexcept
  {
    p.pos = p.spawn_pt;
  }
}
