/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "common/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(std::string const& zone_file, Item_Parser const& items,
           std::string const& enemy_json)
           : zones(zone_file, items), enemy_decl(enemy_json)
  {
  }
}
