/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
#include "../game/Game.h"

#include "View.h"
namespace game { namespace ui
{
  struct Bad_Orientation{};
  Shared_View load(Game&, std::string file) noexcept;
} }
