/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "decl/items.h"
namespace game
{
  struct Player;
  double get_view_radius(Player& p) noexcept;

  int get_additional_defense(Player& p) noexcept;
  int get_additional_damage(Player& p) noexcept;

  bool is_locked_item(Item_Parser& items, Item item) noexcept;
}
