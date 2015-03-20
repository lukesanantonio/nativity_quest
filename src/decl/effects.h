/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "../assets/desc/items.h"

namespace game
{
  struct Player;

  namespace decl
  {
    using assets::Item;
    using assets::Items;

    struct Effects
    {
      explicit Effects(Items* items = nullptr) noexcept : items(items) {}

      double view_radius(Player const&) const noexcept;
      double max_movement(Player const&) const noexcept;

      int additional_defense(Player const&) const noexcept;
      int additional_damage(Player const&) const noexcept;

      bool used_in_navigation(Item) const noexcept;
      bool used_in_combat(Item) const noexcept;
      bool apply_effect(Player&, Item item) const noexcept;

      assets::Items* items;
    };
  }
}
