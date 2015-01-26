/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "../game/Game.h"

#include "cast.h"
#include "View.h"
namespace game { namespace ui
{
  struct Bad_Orientation{};

  Shared_View load(Game&, std::string file) noexcept;

  template <typename T>
  inline std::shared_ptr<T> load_as(Game& g, std::string file) noexcept
  {
    return as<T>(load(g, file));
  }
} }
