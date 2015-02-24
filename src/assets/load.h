/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <vector>
#include <string>

#include "asset.h"

#include "../game/Game.h"

namespace game { namespace assets
{
  void load(Game& game, std::vector<std::string> const& assets) noexcept;
} }
