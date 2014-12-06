/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "SDL.h"
#include "util/surface.h"

#include "Player.h"
#include "decl/zones.h"
namespace game
{
  struct Item_Parser;

  using player_id = short;

  /*!
   * \brief Represents a map image separated into zones housing 6 players.
   *
   * No game state (such as the currently active player) should be here.
   */
  struct Map
  {
    Map(std::string const& zone_json, Item_Parser const&);

    Zone_Parser zones;
    std::array<Player, 6> players;

    double scale; // screen pixels / map pixel
    double mini_scale; // screen pixels / map pixel
  };
}
