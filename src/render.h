/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "common/vec.h"
#include "Map.h"
#include "game/Graphics_Desc.h"
#include "Sprite_Container.h"

namespace game
{
  void render_as_minimap(Graphics_Desc& g, Sprite_Container& sprites,
                         Map const& map, Vec<int> const& start) noexcept;
  void render_with_border(Graphics_Desc& g, SDL_Rect rect);
  bool render_health(Graphics_Desc& g, SDL_Rect box,
                     int life, int max, int change_size,
                     int life_step) noexcept;
}
