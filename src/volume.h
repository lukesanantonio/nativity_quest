/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "pong/common/vector.h"

namespace game
{
  template <typename T = int>
  struct Volume
  {
    pong::math::vector<T> pos;
    T width;
    T height;
  };
}
