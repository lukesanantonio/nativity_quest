/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "Player.h"
#include "decl/items.h"
namespace game
{
  double get_view_radius(Player& p, Item_Parser& items) noexcept
  {
    // Get the torch item.
    static Item_Parser* items_ptr;
    static Item torch;
    if(items_ptr != &items)
    {
      items_ptr = &items;
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
}
