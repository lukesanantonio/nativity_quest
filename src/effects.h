/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
namespace game
{
  struct Player; struct Item_Parser;
  double get_view_radius(Player& p) noexcept;

  int get_additional_defense(Player& p) noexcept;
}
