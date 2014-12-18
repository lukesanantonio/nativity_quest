/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
namespace game
{
  struct Entity_Data
  {
    int cur_life = 0;
    int max_life = 0;

    int defense = 0;
  };

  inline void reset_life(Entity_Data& et) noexcept
  {
    et.cur_life = et.max_life;
  }
}
