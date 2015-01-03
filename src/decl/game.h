/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>

#include "../common/vec.h"
namespace game { namespace decl
{
  struct Game
  {
    Game(std::string json) noexcept;

    std::string title;

    Vec<int> size;

    bool fullscreen;
    bool vsync;
  };
} }
