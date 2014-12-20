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
  double get_max_movement(Player& p) noexcept
  {
    // Get the torch item.
    static Item_Parser* items_ptr;
    static Item canteen;
    if(items_ptr != p.item_parser)
    {
      items_ptr = p.item_parser;
      canteen = items_ptr->get_item("Canteen");
    }

    // Find a canteen in the player's inventory
    using std::begin; using std::end;
    auto item_find = std::find_if(begin(p.inventory), end(p.inventory),
    [&](Item item)
    {
      return item == canteen;
    });

    if(item_find == end(p.inventory))
    {
      return p.max_movement;
    }
    else
    {
      return p.max_movement + 50.0;
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

    int ret = 0;
    for(auto item : p.inventory)
    {
      if(item == ring)
      {
        ret += 1;
      }
      if(item == cloak)
      {
        ret += 2;
      }
    }
    return ret;
  }
  int get_additional_damage(Player& p) noexcept
  {
    static Item_Parser* items_ptr;
    static Item dagger;
    static Item sword;
    static Item axe;
    static Item bow;
    if(items_ptr != p.item_parser)
    {
      items_ptr = p.item_parser;
      dagger = items_ptr->get_item("Dagger");
      sword = items_ptr->get_item("Longsword");
      axe = items_ptr->get_item("Throwing Axe");
      bow = items_ptr->get_item("Bow");
    }

    int ret = 0;

    for(auto item : p.inventory)
    {
      if(item == dagger)
      {
        ret += 1;
      }
      if(item == sword)
      {
        ret += 2;
      }
      if(item == axe)
      {
        ret += 1;
      }
      if(item == bow)
      {
        ret += 1;
      }
    }
    return ret;
  }
  bool is_locked_item(Item_Parser& items, Item item) noexcept
  {
    static Item_Parser* items_ptr;
    static Item wool;
    static Item starlight;
    static Item gold;
    static Item gemstones;
    static Item myrrh;
    static Item incense;
    if(items_ptr != &items)
    {
      items_ptr = &items;
      wool = items_ptr->get_item("Wool");
      starlight = items_ptr->get_item("Starlight");
      gold = items_ptr->get_item("Gold");
      gemstones = items_ptr->get_item("Gemstones");
      myrrh = items_ptr->get_item("Myrrh");
      incense = items_ptr->get_item("Incense");
    }

    if(item == wool || item == starlight || item == gold ||
       item == gemstones || item == myrrh || item == incense)
    {
      return true;
    }

    return false;
  }
  bool can_be_used(Item_Parser& items, Item item) noexcept
  {
    static Item_Parser* items_ptr;
    static Item health;
    static Item haste;
    if(items_ptr != &items)
    {
      items_ptr = &items;
      health = items_ptr->get_item("Potion of Strength");
      haste = items_ptr->get_item("Potion of Haste");
    }

    if(item == health || item == haste)
    {
      return true;
    }

    return false;
  }
  bool can_be_combat_used(Item_Parser& items, Item item) noexcept
  {
    static Item_Parser* items_ptr;
    static Item defense;
    static Item flare;
    if(items_ptr != &items)
    {
      items_ptr = &items;
      defense = items_ptr->get_item("Potion of Defense");
      flare = items_ptr->get_item("Flare");
    }

    if(item == defense || item == flare)
    {
      return true;
    }

    return false;
  }
  void apply_effect(Player& p, Item item) noexcept
  {
    if(item == p.item_parser->get_item("Potion of Strength"))
    {
      reset_life(p.entity_data);
    }
    else if(item == p.item_parser->get_item("Potion of Haste"))
    {
      p.turns_of_haste += 3;
    }
    else if(item == p.item_parser->get_item("Potion of Defense"))
    {
      p.combat_defense = 2;
    }
    else if(item == p.item_parser->get_item("Flare"))
    {
      p.flare = true;
    }
  }
}
