/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "items.h"

#include <algorithm>

#include "../find.h"

#include "../../common/json.h"
#include "../../common/log.h"
namespace game { namespace assets
{
  bool parse_item(rapidjson::Value const& val, Item_Impl& item) noexcept
  {
    if(!has_json_members(val, {"id", "str", "x", "y"}))
    {
      return false;
    }

    item.id = val["id"].GetString();
    item.str = val["str"].GetString();
    item.sprite_pos.x = val["x"].GetInt();
    item.sprite_pos.y = val["y"].GetInt();
    return true;
  }
  void describe(Asset_Ptr<Items> const& asset, Items& items,
                assets::Vector& assets) noexcept
  {
    if_has_member(asset->json, "spritesheet", [&](auto const& val)
    {
      items.spritesheet = find<Image_Asset>(assets, val.GetString());
    });
    if_has_member(asset->json, "sprite_width", [&](auto const& val)
    {
      items.sprite_extents.x = val.GetInt();
    });
    if_has_member(asset->json, "sprite_height", [&](auto const& val)
    {
      items.sprite_extents.y = val.GetInt();
    });

    if_has_member(asset->json, "items", [&](auto const& val)
    {
      for(auto iter = val.Begin(); iter != val.End(); ++iter)
      {
        Item_Impl item;
        if(parse_item(*iter, item))
        {
          items.items.push_back(std::make_shared<Item_Impl>(std::move(item)));
        }
        else
        {
          // TODO: Serialize the bad object represented by *iter.
          log_w("Ignoring invalid item decl object");
        }
      }
    });
  }
  Item Items::get_item(std::string const& str) const noexcept
  {
    // The random string just picks a random item.
    if(str == "random")
    {
      static std::random_device r_device;
      std::mt19937 prng{r_device()};
      std::uniform_int_distribution<int> dist(0, items.size() - 1);
      return items[dist(prng)];
    }

    // Find the item by id.
    using std::begin; using std::end;
    auto item_find = std::find_if(begin(items), end(items),
    [&str](auto item)
    {
      return str == item->id;
    });

    if(item_find == end(items))
    {
      return no::item;
    }

    return *item_find;
  }
} }

