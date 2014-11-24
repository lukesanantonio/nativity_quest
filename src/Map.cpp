/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "util/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(std::string const& map,
           std::string const& items,
           std::string const& zone_json)
           : items(items), zones(zone_json, items), img(map) {}
}
