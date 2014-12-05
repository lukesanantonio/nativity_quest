/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "char.h"

#include "../util/json.h"
#include "../util/except.h"
namespace game
{
    Character_Parser::Character_Parser(std::string json_file)
    {
      auto doc = parse_json(json_file);

      if(!has_json_members(doc, {"sprite", "width", "height", "decl"}))
      {
        throw Bad_Asset{json_file, "Missing members 'sprite', 'width', "
                                   "'height', 'decl' or a combination of "
                                   "these."};
      }

      auto const& decl_arr = doc["decl"];
      short index = 0;
      for(short index = 0; index < infos_.size(); ++index)
      {
        auto& info = infos_[index];

        info.sprite.assign(doc["sprite"].GetString());
        info.src.width = doc["width"].GetInt();
        info.src.height = doc["height"].GetInt();

        auto const& start_json = decl_arr[index]["start"];
        info.src.pos.x = start_json["x"].GetInt();
        info.src.pos.y = start_json["y"].GetInt();

        auto const& center_json = decl_arr[index]["center"];
        info.center.x = center_json["x"].GetInt();
        info.center.y = center_json["y"].GetInt();
      }
    }
}
