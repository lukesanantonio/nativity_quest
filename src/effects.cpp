/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"
#include "decl/items.h"
namespace game
{
  double get_view_radius(Player& p) noexcept
  {
    // Get the torch item.
    static Item_Parser* items_ptr;
    static Item torch;
    if(items_ptr != p.item_parser)
    {
      items_ptr = p.item_parser;
      torch = items_ptr->get_item("Torch");
    }

    // Find a torch in the player's inventory
    using std::begin; using std::end;
    auto torch_find = std::find_if(begin(p.inventory), end(p.inventory),
    [&](auto item)
    {
      return item == torch;
    });

    if(torch_find == end(p.inventory))
    {
      return p.view_radius;
    }
    else
    {
      return p.view_radius + 40;
    }
  }
  int get_additional_defense(Player& p) noexcept
  {
    static Item_Parser* items_ptr;
    static Item ring;
    static Item cloak;
    if(items_ptr != p.item_parser)
    {
      items_ptr = p.item_parser;
      ring = items_ptr->get_item("Ring of Protection");
      cloak = items_ptr->get_item("Cloak of Protection");
    }

    // Find a torch in the player's inventory
    using std::begin; using std::end;
    auto item_find = std::find_if(begin(p.inventory), end(p.inventory),
    [&](auto item)
    {
      return item == ring || item == cloak;
    });

    if(item_find == end(p.inventory))
    {
      return 0;
    }
    else if(*item_find == ring)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
}
