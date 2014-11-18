/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../items.h"
namespace game
{
  struct Item_Parse
  {
    Item_Parse(std::string const& filename);

    std::string spritesheet;
    std::vector<Item> items;
  };
}
