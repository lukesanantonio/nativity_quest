/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <string>

#include "../zones.h"
namespace game
{
  struct Invalid_Class_Reference
  {
    std::string class_str;
  };

  struct Zone_Parser
  {
    Zone_Parser(std::string const& filename);

    std::vector<Zone> zones;
  };
}
