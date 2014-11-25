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

    enum class Direction { Up, Down, Right, Left };
    Direction dir = Direction::Left;

    std::array<Item, 6> inventory;
  };

  SDL_Rect get_sprite_from_direction(Player::Direction);

  void handle_player_event(SDL_Event const& e, Player& p);
}
