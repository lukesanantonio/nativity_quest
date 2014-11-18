/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "items.h"

#include "impl/item_impl.h"
#include "impl/item_parse.h"
namespace game
{
  Item get_item(std::string const& item_name) noexcept
  {
    // TODO handle exceptions.
    static const auto items = Item_Parse{"assets/item.json"};

    using std::begin; using std::end;
    auto item_find = std::find_if(begin(items.items), end(items.items),
    [&item_name](auto item)
    {
      return item_name == item->str;
    });

    if(item_find == end(items.items))
    {
      return no::item;
    }

    return *item_find;
  }
}
