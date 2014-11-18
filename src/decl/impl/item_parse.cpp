/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "item_parse.h"
#include "item_impl.h"

#include "rapidjson/document.h"

#include "../../util/except.h"
#include "../../util/json.h"

namespace game
{
  Item_Impl parse_item(rapidjson::Value const& val) noexcept
  {
    return Item_Impl{val["str"].GetString(),
                     {val["x"].GetInt(), val["y"].GetInt()}};
  }
  Item_Parse::Item_Parse(std::string const& filename)
  {
    auto d = parse_json(filename);

    if(!has_json_members(d, {"spritesheet", "items"}))
    {
      throw Bad_Asset{filename,
                      "Missing 'spritesheet' or 'items' member (or both)"};
    }

    spritesheet.assign(d["spritesheet"].GetString(),
                       d["spritesheet"].GetStringLength());

    for(auto iter = d["items"].Begin(); iter != d["items"].End(); ++iter)
    {
      items.push_back(std::make_shared<Item_Impl>(parse_item(*iter)));
    }
  }
}
