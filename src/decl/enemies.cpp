/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "enemies.h"
#include "../common/json.h"
namespace game { namespace decl
{
  Enemies::Enemies(std::string const& enemy_json)
  {
    auto doc = parse_json(enemy_json);

    for(auto iter = doc.Begin(); iter != doc.End(); ++iter)
    {
      auto const& obj = *iter;

      if(!has_json_members(obj, {"sprite", "id", "str", "defense", "life"}))
      {
        throw Bad_Asset{enemy_json, "Bad enemy listing."};
      }

      auto enemy = Enemy_Impl{obj["sprite"].GetString(),
                              obj["id"].GetString(),
                              obj["str"].GetString(),
                              obj["defense"].GetInt(),
                              obj["life"].GetInt()};

      enemies_.push_back(Enemy{new Enemy_Impl(enemy)});
    }
  }
} }
