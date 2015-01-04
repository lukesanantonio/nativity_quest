/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "players.h"

#include "../common/json.h"
#include "../common/except.h"
namespace game { namespace decl
{
    Players::Players(std::string json_file)
    {
      auto doc = parse_json(json_file);

      if(!has_json_members(doc,
                           {"sprite", "scale", "width", "height", "decl",
                            "player0_item", "player1_item", "player2_item",
                            "player3_item", "player4_item", "player5_item"}))
      {
        throw Bad_Asset{json_file, "Missing members in json object."};
      }

      sprite_.assign(doc["sprite"].GetString());
      sprite_scale_ = doc["scale"].GetDouble();

      starting_items_[0].assign(doc["player0_item"].GetString());
      starting_items_[1].assign(doc["player1_item"].GetString());
      starting_items_[2].assign(doc["player2_item"].GetString());
      starting_items_[3].assign(doc["player3_item"].GetString());
      starting_items_[4].assign(doc["player4_item"].GetString());
      starting_items_[5].assign(doc["player5_item"].GetString());

      player_names_[0].assign(doc["player0_name"].GetString());
      player_names_[1].assign(doc["player1_name"].GetString());
      player_names_[2].assign(doc["player2_name"].GetString());
      player_names_[3].assign(doc["player3_name"].GetString());
      player_names_[4].assign(doc["player4_name"].GetString());
      player_names_[5].assign(doc["player5_name"].GetString());

      auto const& decl_arr = doc["decl"];
      for(short index = 0; index < sprite_info_.size(); ++index)
      {
        auto& sprite_info = sprite_info_[index];

        sprite_info.src.width = doc["width"].GetInt();
        sprite_info.src.height = doc["height"].GetInt();
        sprite_info.src.pos.x = decl_arr[index]["start"]["x"].GetInt();
        sprite_info.src.pos.y = decl_arr[index]["start"]["y"].GetInt();

        sprite_info.center.x = decl_arr[index]["center"]["x"].GetInt();
        sprite_info.center.y = decl_arr[index]["center"]["y"].GetInt();
      }
    }
} }
