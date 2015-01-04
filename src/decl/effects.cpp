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
      if(item == items->get_item("Torch"))
      {
        more = std::max(more, 20.0);
      }
      else if(item == items->get_item("Flare"))
      {
        more = std::max(more, 5.0);
      }
    }

    return p.view_radius + more;
  }
  double Effects::max_movement(Player const& p) const noexcept
  {
    auto canteen = items->get_item("Canteen");

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
      if(item == items->get_item("Ring of Protection"))
      {
        ret += 1;
      }
      if(item == items->get_item("Cloak of Protection"))
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
      if(item == items->get_item("Dagger"))
      {
        ret += 1;
      }
      if(item == items->get_item("Longsword"))
      {
        ret += 2;
      }
      if(item == items->get_item("Throwing Axe"))
      {
        ret += 1;
      }
      if(item == items->get_item("Bow"))
      {
        ret += 1;
      }
    }
    return ret;
  }
  bool Effects::used_in_navigation(Item item) const noexcept
  {
    if(item == items->get_item("Potion of Strength") ||
       item == items->get_item("Potion of Haste"))
    {
      return true;
    }

    return false;
  }
  bool Effects::used_in_combat(Item item) const noexcept
  {
    if(item == items->get_item("Potion of Strength") ||
       item == items->get_item("Potion of Defense") ||
       item == items->get_item("Flare"))
    {
      return true;
    }

    return false;
  }
  bool Effects::apply_effect(Player& p, Item item) const noexcept
  {
    if(item == items->get_item("Potion of Strength"))
    {
      if(p.entity_data.cur_life == p.entity_data.max_life)
      { return false; }
      reset_life(p.entity_data);
    }
    else if(item == items->get_item("Potion of Haste"))
    {
      p.turns_of_haste += 3;
    }
    else if(item == items->get_item("Potion of Defense"))
    {
      p.combat_defense = 2;
    }
    else if(item == items->get_item("Flare"))
    {
      p.flare = true;
    }
    return true;
  }
} }
