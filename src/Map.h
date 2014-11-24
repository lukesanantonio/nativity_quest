/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "SDL.h"
#include "util/surface.h"

#include "common/Cache.h"

#include "Sprite.h"

#include "player.h"
#include "decl/zones.h"
namespace game
{
  /*!
   * \brief Represents a map image separated into zones housing 6 players.
   *
   * No game state (such as the currently active player) should be here.
   */
  struct Map
  {
    Map(std::string const& map,
        std::string const& items,
        std::string const& zone_json);

    std::array<Player, 6> players;

    Item_Parser items;
    Zone_Parser zones;

    Sprite img;
  };
}
