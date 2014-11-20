/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "items.h"

#include "../util/except.h"
#include "../util/json.h"
namespace game
{
  Item_Impl parse_item(rapidjson::Value const& val) noexcept
  {
    return Item_Impl{val["str"].GetString(),
                     {val["x"].GetInt(), val["y"].GetInt()}};
  }
  Item_Parser::Item_Parser(std::string const& filename)
  {
    auto d = parse_json(filename);

    if(!has_json_members(d, {"spritesheet", "items"}))
    {
      throw Bad_Asset{filename,
                      "Missing 'spritesheet' or 'items' member (or both)"};
    }

    spritesheet_.assign(d["spritesheet"].GetString(),
                        d["spritesheet"].GetStringLength());

    for(auto iter = d["items"].Begin(); iter != d["items"].End(); ++iter)
    {
      items_.push_back(std::make_shared<Item_Impl>(parse_item(*iter)));
    }
  }
  std::string Item_Parser::get_spritesheet_filename() const noexcept
  {
    return spritesheet_;
  }
  Item Item_Parser::get_item(std::string const& str) const noexcept
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
}
