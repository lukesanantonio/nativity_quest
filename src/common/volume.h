/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include "vec.h"
namespace game
{
  template <class T = int>
  class Volume
  {
    Vec<T> pos;
    T width;
    T height;
  };
}
