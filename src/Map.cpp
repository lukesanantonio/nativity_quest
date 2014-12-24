/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "common/except.h"

#include "SDL_image.h"
namespace game
{
  Map::Map(decl::Sprites& sprites, std::string map_json,
           std::string items_json, std::string enemies_json) noexcept
           : sprites(sprites), items(items_json), enemies_decl(enemies_json)
  {
    auto map = parse_json(map_json);

    map_sprite.assign(map["sprite"].GetString());
    map_overlay_sprite.assign(map["overlay_sprite"].GetString());
    chest_sprite.assign(map["chest_sprite"].GetString());

    // TODO: Parse spawns.

    scale = map["map_scale"].GetDouble();
    mini_scale = map["minimap_scale"].GetDouble();

    zones.sprite(sprites.get_sprite(map["zones_sprite"].GetString()));
    zones.initialize_zones(items, map["zone_classes"], map["zones"]);
  }
}
