/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
#include <random>
namespace game
{
  static constexpr short damage_table[6][10] = {
    {0, 0, 1, 1, 1, 2, 3, 4, 5, 6},
    {0, 1, 1, 2, 2, 3, 4, 5, 6, 7},
    {1, 2, 2, 3, 3, 4, 5, 6, 7, 8},
    {1, 2, 3, 4, 5, 5, 6, 7, 8, 9},
    {2, 3, 4, 5, 6, 7, 7, 8, 9, 10},
    {3, 3, 4, 6, 6, 7, 7, 8, 9, 10}
  };

  short damage() noexcept
  {
#if 0
    if(power > 10) throw Invalid_Power_Level{};
    if(power < 1) return 0;
    std::mt19937 prng;
    std::uniform_int_distribution<short> gen(0, 1);

    return damage_table[gen(prng)][power - 1];
#endif
    // New combat damage method, basically just from 1-3.
    static std::mt19937 prng{std::random_device{}()};
    static std::uniform_int_distribution<short> gen(1, 3);
    return gen(prng);
  }
}
