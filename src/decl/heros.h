/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <array>
#include <string>

#include "items.h"

namespace game
{
  struct Hero
  {
    std::string player_name;
    std::string hero_name;
    const short power;
    short remaining_life;

    std::array<Item_Type, 5> inventory;
    Item_Type transport_item;

    int visual_id;
  };

  enum class Hero_Id
  {
    Susanna,
    Rafaela,
    Melchior,
    Deborah,
    Casper,
    Balthazar
  };

  Hero instantiate_hero(Hero_Id) noexcept;
}
