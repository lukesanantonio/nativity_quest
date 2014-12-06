/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "util/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(std::string const& zone_file, Item_Parser const& items)
           : zones(zone_file, items),
             // What the actual... TODO fix
             players{Player{zones.get_map_extents()},
                     Player{zones.get_map_extents()},
                     Player{zones.get_map_extents()},
                     Player{zones.get_map_extents()},
                     Player{zones.get_map_extents()},
                     Player{zones.get_map_extents()}} {}
}
