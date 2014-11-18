/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "../items.h"
namespace game
{
  struct Zone_Impl
  {
    std::string str;
    Item required_item;
    short speed_cost;
    bool important;
    short color;
  };
}
