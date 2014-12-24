/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "items.h"

#include <algorithm>

#include "../common/except.h"
#include "../common/json.h"
namespace game { namespace decl
{
  Item_Impl parse_item(rapidjson::Value const& val) noexcept
  {
    return Item_Impl{val["str"].GetString(),
                     {val["x"].GetInt(), val["y"].GetInt()}};
  }
  Items::Items(std::string const& filename)
  {
    auto d = parse_json(filename);

    if(!has_json_members(d, {"spritesheet", "items"}))
    {
      throw Bad_Asset{filename,
                      "Missing 'spritesheet' or 'items' member (or both)"};
    }

    spritesheet_.assign(d["spritesheet"].GetString(),
                        d["spritesheet"].GetStringLength());

    sprite_extents_.x = d["sprite_width"].GetInt();
    sprite_extents_.y = d["sprite_height"].GetInt();

    for(auto iter = d["items"].Begin(); iter != d["items"].End(); ++iter)
    {
      items_.push_back(std::make_shared<Item_Impl>(parse_item(*iter)));
    }
  }
  Item Items::get_item(std::string const& str) const noexcept
  {
    using std::begin; using std::end;
    auto item_find = std::find_if(begin(items_), end(items_),
    [&str](auto item)
    {
      return str == item->str;
    });

    if(item_find == end(items_))
    {
      return no::item;
    }

    return *item_find;
  }
} }
