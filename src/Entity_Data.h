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

  inline int apply_damage(Entity_Data& et, int damage,
                          int mo_defense = 0) noexcept
  {
    auto normalized_damage = std::max(0, damage - et.defense - mo_defense);
    normalized_damage = std::min(et.cur_life, normalized_damage);

    et.cur_life = std::max(0, et.cur_life - normalized_damage);

    return normalized_damage;
  }
}
