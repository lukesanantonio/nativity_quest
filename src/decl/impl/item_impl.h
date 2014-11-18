/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "../../common/vec.h"
namespace game
{
  struct Item_Impl
  {
    std::string str;
    Vec<int> sprite_position;
  };
}
