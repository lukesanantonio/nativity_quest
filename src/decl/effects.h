/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "items.h"

namespace game
{
  struct Player;

  namespace decl
  {
    struct Effects
    {
      double view_radius(Player const&) const noexcept;
      double max_movement(Player const&) const noexcept;

      int additional_defense(Player const&) const noexcept;
      int additional_damage(Player const&) const noexcept;

      bool character_locked(Item) const noexcept;
      bool used_in_navigation(Item) const noexcept;
      bool used_in_combat(Item) const noexcept;
      bool apply_effect(Player&, Item item) const noexcept;

      Items* items = nullptr;
    };
  }
}
