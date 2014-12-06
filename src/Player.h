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
    explicit Player(Vec<int> extents) noexcept;

    // Position on the map.
    Vec<double> pos;

    std::array<Item, 6> inventory;

    Sprite_Impl fog;
  };

  void unfog(Player& p, int radius) noexcept;
}
