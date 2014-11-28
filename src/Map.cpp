/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "util/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(std::string const& items_file, std::string const& zone_json)
           : items(items_file), zones(zone_json, items) {}
}
