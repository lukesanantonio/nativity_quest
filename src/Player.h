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

#include "Entity_Data.h"

namespace game
{
  struct Player
  {
    // Position on the map.
    Vec<double> pos;

    std::array<Item, 6> inventory;

    double view_radius = 85.0;
    Sprite_Impl fog;

    Entity_Data entity_data;

    double moved;
    double max_movement;

    int turns_of_haste = 0;

    bool done = false;

    Item_Parser* item_parser;
  };

  void reset_fog(Player&, Vec<int> extents) noexcept;
  void unfog(Player&) noexcept;
}
