/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <memory>
#include "../common/vec.h"
namespace game
{
  struct Zone_Impl;

  using Zone = std::shared_ptr<const Zone_Impl>;
  Zone get_zone(Vec<int> pos);

  namespace no
  {
    const Zone zone;
  }
}
