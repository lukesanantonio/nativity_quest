/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#include "combat.h"
#include <random>
namespace game { namespace decl
{
  short damage() noexcept
  {
    // New combat damage method, basically just from 1-3.
    static std::mt19937 prng{std::random_device{}()};
    static std::uniform_int_distribution<short> gen(1, 3);
    return gen(prng);
  }
} }
