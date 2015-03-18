/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "effects.h"
#include "../Player.h"
namespace game { namespace decl
{
  double Effects::view_radius(Player const& p) const noexcept
  {
    double more = 0.0;
    for(auto& item : p.inventory)
    {
      if(item == items->get_item("item_torch"))
      {
        more = std::max(more, 20.0);
      }
      else if(item == items->get_item("item_flare"))
      {
        more = std::max(more, 5.0);
      }
    }

    return p.view_radius + more;
  }
  double Effects::max_movement(Player const& p) const noexcept
  {
    auto canteen = items->get_item("item_canteen");

    auto ret = p.max_movement;
    for(auto item : p.inventory)
    {
      if(item == canteen)
      {
        ret += 20.0;
      }
    }
    return ret;
  }
  int Effects::additional_defense(Player const& p) const noexcept
  {
    int ret = 0;
    for(auto item : p.inventory)
    {
      if(item == items->get_item("item_ring_of_protection"))
      {
        ret += 1;
      }
      if(item == items->get_item("item_cloak_of_protection"))
      {
        ret += 2;
      }
    }
    return std::min(2, ret);
  }
  int Effects::additional_damage(Player const& p) const noexcept
  {
    int ret = 0;
    for(auto item : p.inventory)
    {
      if(item == items->get_item("item_dagger"))
      {
        ret += 1;
      }
      if(item == items->get_item("item_longsword"))
      {
        ret += 2;
      }
      if(item == items->get_item("item_throwing_axe"))
      {
        ret += 1;
      }
      if(item == items->get_item("item_bow"))
      {
        ret += 1;
      }
    }
    return ret;
  }
  bool Effects::used_in_navigation(Item item) const noexcept
  {
    if(item == items->get_item("item_potion_of_strength") ||
       item == items->get_item("item_potion_of_haste"))
    {
      return true;
    }

    return false;
  }
  bool Effects::used_in_combat(Item item) const noexcept
  {
    if(item == items->get_item("item_potion_of_strength") ||
       item == items->get_item("item_potion_of_defense") ||
       item == items->get_item("item_flare"))
    {
      return true;
    }

    return false;
  }
  bool Effects::apply_effect(Player& p, Item item) const noexcept
  {
    if(item == items->get_item("item_potion_of_strength"))
    {
      if(p.entity_data.cur_life == p.entity_data.max_life)
      { return false; }
      reset_life(p.entity_data);
    }
    else if(item == items->get_item("item_potion_of_haste"))
    {
      p.turns_of_haste += 3;
    }
    else if(item == items->get_item("item_potion_of_defense"))
    {
      p.combat_defense = 2;
    }
    else if(item == items->get_item("item_flare"))
    {
      p.flare = true;
    }
    return true;
  }
} }
