/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "enemies.h"
#include "../util/json.h"
namespace game
{
  Enemy_Parser::Enemy_Parser(std::string const& enemy_json)
  {
    auto doc = parse_json(enemy_json);

    for(auto iter = doc.Begin(); iter != doc.End(); ++iter)
    {
      auto const& obj = *iter;

      if(!has_json_members(obj, {"sprite", "name", "power", "life"}))
      {
        throw Bad_Asset{enemy_json, "Bad enemy listing."};
      }

      auto enemy = Enemy_Impl{obj["sprite"].GetString(),
                              obj["name"].GetString(),
                              obj["power"].GetInt(), obj["life"].GetInt()};

      enemies_.push_back(Enemy{new Enemy_Impl(enemy)});
    }
  }
}
