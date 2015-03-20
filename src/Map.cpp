/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Map.h"
#include "common/except.h"

#include "assets/assets.h"

#include "SDL_image.h"
namespace game
{
  template <typename D>
  Vec<int> parse_point(D const& doc) noexcept
  {
    return {doc["x"].GetInt(), doc["y"].GetInt()};
  }

  using assets::describe;
  using assets::Items;
  using assets::Image_Asset;

  Map::Map(Game& g, std::string map_json,
           std::string items_json, std::string enemies_json) noexcept
           : game(g), items(describe<Items>(g.assets, items_json)),
             enemies_decl(enemies_json)
  {
    auto map = parse_json(map_json);

    map_sprite = get_asset<Image_Asset>(g, map["sprite"].GetString());
    map_overlay_sprite =
                  get_asset<Image_Asset>(g, map["overlay_sprite"].GetString());
    chest_sprite = get_asset<Image_Asset>(g, map["chest_sprite"].GetString());

    // Spawns
    auto& map_spawns = map["spawns"];
    for(auto iter = map_spawns.Begin(); iter < map_spawns.End(); ++iter)
    {
      if(iter->HasMember("enemy"))
      {
        auto enemy_decl = enemies_decl.get_enemy((*iter)["enemy"].GetString());

        auto enemy_inst = Enemy_Instance{enemy_decl};
        enemy_inst.pos = parse_point((*iter)["pos"]);

        enemies.push_back(enemy_inst);
      }
      else if(iter->HasMember("chest"))
      {
        auto chest_item = items.get_item((*iter)["chest"].GetString());
        chests.push_back(Chest{chest_item, true, parse_point((*iter)["pos"])});
      }
    }

    players[0].spawn_pt = parse_point(map["player0_spawn"]);
    players[1].spawn_pt = parse_point(map["player1_spawn"]);
    players[2].spawn_pt = parse_point(map["player2_spawn"]);
    players[3].spawn_pt = parse_point(map["player3_spawn"]);
    players[4].spawn_pt = parse_point(map["player4_spawn"]);
    players[5].spawn_pt = parse_point(map["player5_spawn"]);

    scale = map["map_scale"].GetDouble();
    mini_scale = map["minimap_scale"].GetDouble();

    auto zsasst = get_asset<assets::Image_Asset>(game,
                                              map["zones_sprite"].GetString());
    zones.sprite(zsasst->image.surface.get());

    zones.initialize_zones(items, map["zone_classes"], map["zones"]);

    for(auto& player : players)
    {
      player.entity_data.max_life = 15;
      reset_life(player.entity_data);
    }
  }
}
