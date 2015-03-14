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

      sprite_srcs_[0].assign(doc["player0_sprite"].GetString());
      sprite_srcs_[1].assign(doc["player1_sprite"].GetString());
      sprite_srcs_[2].assign(doc["player2_sprite"].GetString());
      sprite_srcs_[3].assign(doc["player3_sprite"].GetString());
      sprite_srcs_[4].assign(doc["player4_sprite"].GetString());
      sprite_srcs_[5].assign(doc["player5_sprite"].GetString());

      auto const& decl_arr = doc["decl"];
      for(short index = 0; index < (short) orient_info_.size(); ++index)
      {
        auto& orient_info = orient_info_[index];

        orient_info.src.width = doc["width"].GetInt();
        orient_info.src.height = doc["height"].GetInt();
        orient_info.src.pos.x = decl_arr[index]["start"]["x"].GetInt();
        orient_info.src.pos.y = decl_arr[index]["start"]["y"].GetInt();

        orient_info.center.x = decl_arr[index]["center"]["x"].GetInt();
        orient_info.center.y = decl_arr[index]["center"]["y"].GetInt();
      }
    }
} }
