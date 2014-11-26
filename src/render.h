/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once

#include "volume.h"
#include "Map.h"
#include "Graphics_Desc.h"

namespace game
{
  void render_as_minimap(Graphics_Desc& g,
                         Map const& map,
                         Volume<> vol) noexcept;
  void render_viewport(Graphics_Desc& g, Map const& map, Volume<> viewport,
                       Volume<> destination) noexcept;

  void use_viewport_scale(SDL_Rect& rect,
                          double xscale, double yscale) noexcept;
}
