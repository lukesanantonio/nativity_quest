/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "volume.h"
#include "Map.h"
#include "Graphics_Desc.h"
#include "Sprite_Container.h"

namespace game
{
  void render_as_minimap(Graphics_Desc& g, Sprite_Container& sprites,
                         Map const& map,
                         pong::math::vector<int> const& start) noexcept;
}
