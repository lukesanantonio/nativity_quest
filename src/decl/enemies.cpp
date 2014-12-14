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
      if(!has_all_members(doc, {"sprite", "name", "power", "life"}))
      {
        throw Bad_Asset{enemy_json, "Bad enemy listing."};
      }

      auto enemy = Enemy_Impl{doc["sprite"].GetString(),
                              doc["name"].GetString(),
                              doc["power"].GetInt(), doc["life"].GetInt()};

      enemies_.push_back(Enemy{new Enemy_Impl{enemy}});
    }
  }
}
