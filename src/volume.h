/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "vec.h"

namespace game
{
  template <typename T = int>
  struct Volume
  {
    Vec<T> pos;
    T width;
    T height;
  };
}
